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

#include <stdint.h>
#include "stm32f746g_register.h"

//#define MODE_STAND_ALONE

#ifdef __cplusplus
extern "C" {
#endif

void SystemInit(void);
uint8_t usart1ReceiveChar(void);
void usart1SendChar(const uint8_t character);
void usart1SendBuffer(const uint8_t* message);
void toggleLED1(void);
void showLogo(void);
uint32_t getRandomData(void);
uint32_t testMemoryDMA(uint16_t data);
void FillRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);

#ifdef __cplusplus
}
#endif

#endif
