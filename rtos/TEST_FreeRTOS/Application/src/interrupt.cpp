/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : interrupt.cpp
 * Project  : Minimum RTOS platform
 * Date     : 2021/9/5
 * Author   : WangYu
 *
 **********************************************************************/
#include "stm32f746xx.h"
#include "platform.h"
#include "EHmiEvent.h"
#include "interrupt.h"

#ifdef MODE_STAND_ALONE
extern char character;

// Timer7 500ms interrupt
void TIM7_IRQHandler(void)
{
	writeRegThenWait(TIM7_SR, 0b0, 0, 1); // attention : here must reset UIF !!!!
	toggleLED1();
}

// UART1 receive interrupt for debug
void USART1_IRQHandler(void)
{
	if (readRegister(USART1_ISR, 5, 1) == 1) { // is RXNE set?
		character = (char)(readRegister(USART1_RDR, 0, 8));
		usart1SendChar(character); // echo received character
	}
}
#endif

// PI13(LCD_INT) interrupt from FT5336
void EXTI15_10_IRQHandler(void)
{
	if((EXTI->PR & EXTI_PR_PR13_Msk) != 0) { // from PI13(LCD_INT)
		if (pHmi->IsReady()) {
			uint8_t gestureID = readFT5336GestureID();
			uint8_t ptsNumber = readFT5336PointNum();
			uint8_t eventFlag = readFT5336EventFlag();
			if (gestureID == 0x00 && ptsNumber == 1 && eventFlag < 3) {
				uint32_t x = (uint32_t)readFT5336PointX();
				uint32_t y = (uint32_t)readFT5336PointY();
				// eventFlag = 0:DOWN, 1:UP, 2:MOVE, 3:RESERVED
				EHmiEventType type = static_cast<EHmiEventType>(HMI_EV_TOUCH_DOWN + eventFlag);
				EHmiEvent ev(type, x, y);
				xSemaphoreTakeFromISR(pHmi->Mutex(), 0); // the second parameter is different from "xSemaphoreTake"
				pHmi->SendQueueFromISR(ev);
				xSemaphoreGiveFromISR(pHmi->Mutex(), 0);
			}
		}
	}
	EXTI->PR |= EXTI_PR_PR13;
}

// SDMMC interrupt from microSD card
void SDMMC1_IRQHandler(void)
{
	// Check SD card transfer error
	if (SDMMC1->STA & SDMMC_STA_DCRCFAIL_Msk) {
		TRACE("SD card data CRC check failed !\r\n");
	}
	else if (SDMMC1->STA & SDMMC_STA_DTIMEOUT_Msk) {
		TRACE("SD card data transfer is time out !\r\n");
	}
	else if (SDMMC1->STA & SDMMC_STA_TXUNDERR_Msk) {
		TRACE("SD card data transfer has a TX FIFO underflow error !\r\n");
	}
	else if (SDMMC1->STA & SDMMC_STA_RXOVERR_Msk) {
		TRACE("SD card data transfer has a RX FIFO overrun error !\r\n");
	}
	else {
		;
	}

	uint32_t itMask	=	SDMMC_MASK_CCRCFAILIE_Msk |
						SDMMC_MASK_DTIMEOUTIE_Msk |
						SDMMC_MASK_RXOVERRIE_Msk |
						SDMMC_MASK_TXUNDERRIE_Msk |
						SDMMC_MASK_DATAENDIE_Msk;
	SDMMC1->MASK	&=	~itMask;
	SDMMC1->DCTRL	&=	~(SDMMC_DCTRL_DMAEN_Msk | SDMMC_DCTRL_DTEN_Msk);

	// Here handle the process after writing complete
	if (SDMMC1->STA & SDMMC_STA_DATAEND_Msk) {
		if (sdOpStatus == SD_OP_MULTI_BLOCK_WRITE) {
			sdmmcSendCmd(SD_CMD_STOP_TRANSMISSION, SD_RESPONSE_R1, 0);
		}
		sdOpStatus = SD_OP_IDLE;
	}

	SDMMC1->ICR		|=	SDMMC1->STA;
}

// DMA interrupt for microSD card RX
void DMA2_Stream3_IRQHandler(void)
{
	// Check SD card DMA transfer error
	if (DMA2->LISR & DMA_LISR_TEIF3_Msk) {
		TRACE("SD card RX DMA transfer failed !\r\n");
	}
	else if (DMA2->LISR & DMA_LISR_FEIF3_Msk) {
		TRACE("SD card RX DMA FIFO has an error !\r\n");
	}
	else {
		;
	}

	// Send CMD12 to stop data transfer from SD card
	if (sdOpStatus == SD_OP_MULTI_BLOCK_READ) {
		sdmmcSendCmd(SD_CMD_STOP_TRANSMISSION, SD_RESPONSE_R1, 0);
	}

	DMA2->LIFCR		|=	DMA_LIFCR_CTCIF3_Msk |
						DMA_LIFCR_CTEIF3_Msk |
						DMA_LIFCR_CFEIF3_Msk;
	uint32_t itMask	=	SDMMC_MASK_CCRCFAILIE_Msk |
						SDMMC_MASK_DTIMEOUTIE_Msk |
						SDMMC_MASK_RXOVERRIE_Msk |
						SDMMC_MASK_TXUNDERRIE_Msk;
	SDMMC1->MASK	&=	~itMask;
	SDMMC1->ICR		|=	SDMMC1->STA;
	SDMMC1->DCTRL	&=	~(SDMMC_DCTRL_DMAEN_Msk | SDMMC_DCTRL_DTEN_Msk);

	sdOpStatus = SD_OP_IDLE;
}

// DMA interrupt for microSD card TX
void DMA2_Stream6_IRQHandler(void)
{
	// Check SD card DMA transfer result
	if (DMA2->HISR & DMA_HISR_TCIF6_Msk) {
		// Clear all DMA interrupt flag
		DMA2->HIFCR	|=	DMA_HIFCR_CTCIF6_Msk |
						DMA_HIFCR_CTEIF6_Msk |
						DMA_HIFCR_CFEIF6_Msk;
		// Pay attention !!!
		// In SD card writing handler, DMA transfer is just take place between ram and SDMMC FIFO.
		// After DMA transfer completed, transfer from SDMMC to SD card must be wait.
		// Here start SDMMC DATAEND interrupt to handle process after writing complete.
		SDMMC1->MASK |=	SDMMC_MASK_DATAENDIE_Msk;
		return;
	}
	else if (DMA2->HISR & DMA_HISR_FEIF6_Msk) {
		//if (sdOpStatus == SD_OP_MULTI_BLOCK_WRITE) {
			DMA2->HIFCR |= DMA_HIFCR_CFEIF6_Msk;
			return;
		//}
		//else {
		//	TRACE("SD card TX DMA FIFO has an error !\r\n");
		//}
	}
	else if (DMA2->HISR & DMA_HISR_TEIF6_Msk) {
		TRACE("SD card TX DMA transfer failed !\r\n");
	}
	else {
		;
	}

	DMA2->HIFCR		|=	DMA_HIFCR_CTCIF6_Msk |
						DMA_HIFCR_CTEIF6_Msk |
						DMA_HIFCR_CFEIF6_Msk;
	SDMMC1->DCTRL	&=	~(SDMMC_DCTRL_DMAEN_Msk | SDMMC_DCTRL_DTEN_Msk);
}
