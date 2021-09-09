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
#include "stm32f746g_disco_driver.h"

#ifdef MODE_STAND_ALONE
extern char character;

// Timer7 500ms interrupt
void TIM7_IRQHandler(void)
{
	*((uint32_t *)TIM7_SR) &= (uint32_t)~0x00000001; // attention : here must reset UIF !!!!

	toggleLED1();
}

// UART1 receive interrupt for debug
void USART1_IRQHandler(void)
{
	if((*((uint32_t *)USART1_ISR) & 0x00000020) == 0x00000020) { // is RXNE set?
		character = (char)(*((uint32_t *)USART1_RDR) & 0x000000FF);
		usart1SendChar(character); // echo received character
	}
}
#endif
