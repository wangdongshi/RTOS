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
#include "stm32f746g_register.h"
#include "stm32f746g_disco_driver.h"

typedef unsigned long	uint32_t;

extern char character;

#ifndef LED1_FLICKER_IN_TASK
// Timer7 500ms interrupt
void TIM7_IRQHandler(void)
{
	*((uint32_t *)TIM7_SR) &= (uint32_t)~0x00000001; // attention : here must reset UIF !!!!

	toggleLED1();
}
#endif

// UART1 receive interrupt for debug
void USART1_IRQHandler(void)
{
	if((*((uint32_t *)USART1_ISR) & 0x00000020) == 0x00000020) { // is RXNE set?
		character = (char)(*((uint32_t *)USART1_RDR) & 0x000000FF);
		usart1SendChar(character); // echo received character
	}
}
