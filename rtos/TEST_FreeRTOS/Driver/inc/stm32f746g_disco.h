/**********************************************************************
 * Copyright (c) 2018 - 2021 by Wang Yu
 * All rights reserved
 *
 * Filename:  stm32f746g_disco.h
 * Project:   Minimum RTOS platform
 * Date:      2021/9/5
 * Author:    Wang Yu
 *
 **********************************************************************/
#ifndef STM32F746G_DISCO_DRIVER_H
#define STM32F746G_DISCO_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Font type definition
#define FONT_SMALL				(0)
#define FONT_MIDDLE				(1)

#define MODE_TEST_DRIVER
//#define MODE_STAND_ALONE

// Defined in link script
extern uint32_t _ssdram; // SDRAM start address
extern uint32_t _sdram_size;

void SystemInit(void);

uint8_t usart1ReceiveChar(void);
void usart1SendChar(const uint8_t character);
void usart1Send(const uint8_t* message);

void i2c3Write1Byte(uint8_t slaveAddr, uint8_t devAddr, uint8_t data);
uint8_t i2c3Read1Byte(uint8_t slaveAddr, uint8_t devAddr);

void toggleLED1(void);
void showLogo(void);
void checkDMA2D(void);
uint16_t checkSDRAM(void);
uint16_t checkTouchPanel(void);

void fillRect(
		const uint16_t x,
		const uint16_t y,
		const uint16_t w,
		const uint16_t h,
		const uint32_t color);
void drawImage(
		const uint16_t x,
		const uint16_t y,
		const uint16_t w,
		const uint16_t h,
		const uint32_t addr);
void drawString(
		const uint16_t x,
		const uint16_t y,
		const char* const string,
		const uint32_t foreColor,
		const uint32_t backColor,
		const uint8_t  fontType);

uint32_t getRandomData(void);
uint32_t checkDMA(uint16_t data);

#ifdef __cplusplus
}
#endif

#endif
