/**********************************************************************
 * Copyright (c) 2018 - 2021 by Wang Yu
 * All rights reserved
 *
 * Filename:  debug.c
 * Project:   Minimum RTOS platform
 * Date:      2018/11/26
 * Author:    Wang Yu
 *
 **********************************************************************/
#include <stdio.h>
#include <stdarg.h>
#include "stm32f746g_disco.h"
#include "debug.h"

#ifdef DEBUG

#define MAX_UART_OUTPUT_BUFF 256

uint8_t debugTaceBuf[MAX_UART_OUTPUT_BUFF];

void TRACE(const char *format, ...)
{
    printf(format);
}

#endif
