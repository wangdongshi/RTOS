/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename:  interrupt.c
 * Project:   Minimum RTOS platform
 * Date:      2021/9/5
 * Author:    WangYu
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
	//char* event[4] = {"Finger press", "Finger up", "Finger move", "Reserved"};
	if((EXTI->PR & EXTI_PR_PR13_Msk) != 0) { // from PI13(LCD_INT)
		uint8_t gestureID = readFT5336GestureID();
		uint8_t ptsNumber = readFT5336PointNum();
		uint8_t eventFlag = readFT5336EventFlag();
		if (gestureID == 0x00 && ptsNumber == 1 && eventFlag == 0) {
			uint16_t x	= readFT5336PointX();
			uint16_t y	= readFT5336PointY();
			uint32_t param = x << 16 | y;
			EHmiEvent ev(HMI_EV_KEYDOWN, (unsigned long)param);
			xSemaphoreTake(pHmi->Mutex(), 0);
			pHmi->SetReady(true);
			pHmi->AddQueue(ev);
			xSemaphoreGive(pHmi->Mutex());
			//char* space = eventFlag % 2 ? "\t\t" : "\t";
			//printf("[INFO] Touch event (%s) arise, %s(%d, %d)\r\n", event[eventFlag], space, x, y);
		}
	}
	EXTI->PR |= EXTI_PR_PR13;
}
