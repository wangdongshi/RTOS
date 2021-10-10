/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : stm32f746g_disco.h
 * Project  : Minimum RTOS platform
 * Date     : 2021/9/5
 * Author   : WangYu
 *
 **********************************************************************/
#ifndef __STM32F746G_DISCO_H__
#define __STM32F746G_DISCO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

#define MODE_TEST_DRIVER
//#define MODE_STAND_ALONE

#define LCD_ACTIVE_WIDTH		(480)
#define LCD_ACTIVE_HEIGHT		(272)

typedef enum {
	FONT_SMALL,
	FONT_MIDDLE
} FONT_TYPE;

typedef enum {
	LAYER_BG,
	LAYER_FG
} LAYER_TYPE;

typedef enum {
	COLOR_ARGB8888	= 0b0000,
	COLOR_RGB888	= 0b0001,
	COLOR_RGB565	= 0b0010,
	COLOR_ARGB1555	= 0b0011,
	COLOR_ARGB4444	= 0b0100,
	COLOR_L8		= 0b0101,
	COLOR_AL44		= 0b0110,
	COLOR_AL88		= 0b0111,
	COLOR_L4		= 0b1000,
	COLOR_A8		= 0b1001,
	COLOR_A4		= 0b1010
} RGB_TYPE;

// Defined in link script
extern uint32_t _ssdram; // SDRAM start address
extern uint32_t _sdram_size;

void SystemInit(void);

uint8_t usart1ReceiveChar(void);
void usart1SendChar(const uint8_t character);
void usart1Send(const uint8_t* message);

void i2c3Write1Byte(uint8_t slaveAddr, uint8_t devAddr, uint8_t data);
uint8_t i2c3Read1Byte(uint8_t slaveAddr, uint8_t devAddr);

bool_t isSDCardInsert(void);

void toggleLED1(void);
void showLogo(void);
void checkDMA2D(void);
bool_t checkSDRAM(void);
bool_t checkTouchPanel(void);

void fillRect(
		const uint16_t x,
		const uint16_t y,
		const uint16_t w,
		const uint16_t h,
		const uint32_t color,
		const LAYER_TYPE layer);
void drawImage(
		const uint16_t x,
		const uint16_t y,
		const uint16_t w,
		const uint16_t h,
		const uint32_t addr,
		const RGB_TYPE pfc,
		const LAYER_TYPE layer);
void drawChar(
		const uint16_t x,
		const uint16_t y,
		const uint32_t foreColor,
		const uint32_t backColor,
		const uint16_t symbol,
		const FONT_TYPE fontType,
		const LAYER_TYPE layer);
void drawString(
		const uint16_t x,
		const uint16_t y,
		const uint32_t foreColor,
		const uint32_t backColor,
		const char* const string,
		const FONT_TYPE fontType,
		const LAYER_TYPE layer);

uint32_t getRandomData(void);
bool_t checkDMA(uint16_t data);

#ifdef __cplusplus
}
#endif

#endif // __STM32F746G_DISCO_H__
