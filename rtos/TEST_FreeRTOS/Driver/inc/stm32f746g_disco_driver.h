/**********************************************************************
 * Copyright (c) 2018 - 2021 by Wang Yu
 * All rights reserved
 *
 * Filename:  stm32f746g_disco_driver.h
 * Project:   Minimum RTOS platform
 * Date:      2021/9/5
 * Author:    Wang Yu
 *
 **********************************************************************/
#ifndef STM32F746G_DISCO_DRIVER_H
#define STM32F746G_DISCO_DRIVER_H

#define LED1_FLICKER_IN_TASK

void initBoard(void);
void toggleLED1(void);
void usart1SendChar(const char character);
void usart1SendBuffer(const char* message);

#endif
