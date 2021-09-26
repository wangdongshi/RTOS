/**********************************************************************
 * Copyright (c) 2018 - 2021 by Wang Yu
 * All rights reserved
 *
 * Filename:  interrupt.c
 * Project:   Minimum RTOS platform
 * Date:      2021/9/5
 * Author:    Wang Yu
 *
 **********************************************************************/
#include <stdio.h>
#include "assert_param.h"
#include "stm32f746xx.h"
#include "ft5336.h"
#include "stm32f746g_disco.h"

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
	char* event[4] = {"Finger press", "Finger up", "Finger move", "Reserved"};
	if((EXTI->PR & EXTI_PR_PR13_Msk) != 0) { // from PI13(LCD_INT)
		uint8_t gestureID = readFT5336GestureID();
		uint8_t ptsNumber = readFT5336PointNum();
		uint8_t eventFlag = readFT5336EventFlag();
		if (gestureID == 0x00 && ptsNumber == 1 && eventFlag < 3) {
			uint16_t x	= readFT5336PointX();
			uint16_t y	= readFT5336PointY();
			printf("Touch event (%s) arise, \t(%d, %d)\r\n", event[eventFlag], x, y);
		}
	}
	EXTI->PR |= EXTI_PR_PR13;
}
