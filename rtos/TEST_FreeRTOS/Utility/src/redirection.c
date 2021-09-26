/**********************************************************************
 * Copyright (c) 2018 - 2021 by Wang Yu
 * All rights reserved
 *
 * Filename:  redirection.c
 * Project:   Minimum RTOS platform
 * Date:      2021/9/6
 * Author:    Wang Yu
 *
 **********************************************************************/
#include <stdio.h>
#include "stm32f746g_disco.h"

#ifndef MODE_STAND_ALONE

// I/O redirect
int _write(int file, char *ptr, int len)
{
    for (int i = 0; i < len; i++) {
    	usart1SendChar((const uint8_t)(*ptr++ & 0xFF));
    }
    return len;
}

/*
int __io_putchar(int ch)
{
	while((*((uint32_t *)USART1_ISR) & 0x00000040) == 0); // wait TC set
	*((uint32_t *)USART1_TDR) = (uint32_t)ch; // set character to TX buffer
	//usart1SendChar((const uint8_t)(ch & 0xFF));
	return ch;
}

int __io_getchar(FILE *f)
{
	return usart1ReceiveChar();
}
*/

#endif
