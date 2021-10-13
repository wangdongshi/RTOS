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
	// Disable SDMMC interrupt
	uint32_t itMask =	SDMMC_MASK_CCRCFAILIE_Msk |
						SDMMC_MASK_DTIMEOUTIE_Msk |
						SDMMC_MASK_RXOVERRIE_Msk |
						SDMMC_MASK_DATAENDIE_Msk;
	SDMMC1->MASK &= ~itMask;

	// Result processing
	//if (SDMMC1->STA & SDMMC_STA_DATAEND_Msk) {
	//}

	// Clear all error bit flags
	SDMMC1->ICR = SDMMC1->STA;
}

// DAM interrupt for microSD card RX
void DMA2_Stream3_IRQHandler(void)
{
	// Disable SDMMC DMA interrupt
	SDMMC1->DCTRL &= ~SDMMC_DCTRL_DMAEN_Msk;

	// Result processing
	if (DMA2->LISR & DMA_LISR_TCIF3_Msk) {
		// Send CMD12 to stop data transfer from SD card
		sdmmcSendCmd(SD_CMD_STOP_TRANSMISSION, SD_RESPONSE_R1, 0);
	}

	// Clear SDMMC RX DMA interrupt flag
	DMA2->LIFCR |= 	DMA_LIFCR_CTCIF3_Msk |
					DMA_LIFCR_CTEIF3_Msk |
					DMA_LIFCR_CDMEIF3_Msk |
					DMA_LIFCR_CFEIF3_Msk;
}

// DAM interrupt for microSD card TX
void DMA2_Stream6_IRQHandler(void)
{

}
