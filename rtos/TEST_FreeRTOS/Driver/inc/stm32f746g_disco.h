/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename:  stm32f746g_disco.h
 * Project:   Minimum RTOS platform
 * Date:      2021/9/5
 * Author:    WangYu
 *
 **********************************************************************/
#ifndef __STM32F746G_DISCO_H__
#define __STM32F746G_DISCO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Font type definition
#define LAYER_BG				(0) // background layer
#define LAYER_FG				(1) // foreground layer

#define FONT_SMALL				(0)
#define FONT_MIDDLE				(1)

#define COLOR_ARGB8888			(0b0000)
#define COLOR_RGB888			(0b0001)
#define COLOR_RGB565			(0b0010)
#define COLOR_ARGB1555			(0b0011)
#define COLOR_ARGB4444			(0b0100)
#define COLOR_L8				(0b0101)
#define COLOR_AL44				(0b0110)
#define COLOR_AL88				(0b0111)
#define COLOR_L4				(0b1000)
#define COLOR_A8				(0b1001)
#define COLOR_A4				(0b1010)

#define LCD_ACTIVE_WIDTH		(480)
#define LCD_ACTIVE_HEIGHT		(272)

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
		const uint32_t color,
		const uint8_t  layer);
void drawImage(
		const uint16_t x,
		const uint16_t y,
		const uint16_t w,
		const uint16_t h,
		const uint32_t addr,
		const uint8_t  format,
		const uint8_t  layer);
void drawChar(
		const uint16_t x,
		const uint16_t y,
		const uint32_t foreColor,
		const uint32_t backColor,
		const uint16_t symbol,
		const uint8_t  fontType,
		const uint8_t  layer);
void drawString(
		const uint16_t x,
		const uint16_t y,
		const uint32_t foreColor,
		const uint32_t backColor,
		const char* const string,
		const uint8_t  fontType,
		const uint8_t  layer);

uint32_t getRandomData(void);
uint32_t checkDMA(uint16_t data);

#ifdef __cplusplus
}
#endif

#endif // __STM32F746G_DISCO_H__
