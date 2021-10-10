/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : ft5336.c
 * Project  : Minimum RTOS platform
 * Date     : 2021/9/20
 * Author   : WangYu
 *
 **********************************************************************/
#include <string.h>
#include <stdio.h>
#include "types.h"
#include "ft5336.h"
#include "stm32f746g_disco.h"

// FT5336 IIC slave address
#define FT5336_I2C_SLAVE_ADDR	(0x70)

// FT5336 internal register definition
#define FT5336_OP_DEVICE_MODE	(0x00)
#define FT5336_OP_GEST_ID		(0x01)
#define FT5336_OP_TD_STATUS		(0x02) // Number of touch points
#define FT5336_OP_TOUCH1_XH		(0x03)
#define FT5336_OP_TOUCH1_XL		(0x04)
#define FT5336_OP_TOUCH1_YH		(0x05)
#define FT5336_OP_TOUCH1_YL		(0x06)
#if 0
#define FT5336_OP_TOUCH2_XH		(0x09)
#define FT5336_OP_TOUCH2_XL		(0x0A)
#define FT5336_OP_TOUCH2_YH		(0x0B)
#define FT5336_OP_TOUCH2_YL		(0x0C)
#define FT5336_OP_TOUCH3_XH		(0x0F)
#define FT5336_OP_TOUCH3_XL		(0x10)
#define FT5336_OP_TOUCH3_YH		(0x11)
#define FT5336_OP_TOUCH3_YL		(0x12)
#define FT5336_OP_TOUCH4_XH		(0x15)
#define FT5336_OP_TOUCH4_XL		(0x16)
#define FT5336_OP_TOUCH4_YH		(0x17)
#define FT5336_OP_TOUCH4_YL		(0x18)
#define FT5336_OP_TOUCH5_XH		(0x1B)
#define FT5336_OP_TOUCH5_XL		(0x1C)
#define FT5336_OP_TOUCH5_YH		(0x1D)
#define FT5336_OP_TOUCH5_YL		(0x1E)
#endif
#define FT5336_OP_G_CIPHER		(0xA3)
#define FT5336_OP_G_MODE		(0xA4) // the interrupt status to host
#define FT5336_OP_G_PMODE		(0xA5)
#define FT5336_OP_G_FIRMID		(0xA6)
#define FT5336_OP_G_STATE		(0xA7)
#define FT5336_OP_G_FT5210ID	(0xA8)
#define FT5336_OP_G_ERR			(0xA9)

static void writeFTReg(uint8_t addr, uint8_t data);
static uint8_t readFTReg(uint8_t addr);

// mapping slave address function
static void writeFTReg(uint8_t addr, uint8_t data)
{
	i2c3Write1Byte(FT5336_I2C_SLAVE_ADDR, addr, data);
}

static uint8_t readFTReg(uint8_t addr)
{
	return i2c3Read1Byte(FT5336_I2C_SLAVE_ADDR, addr);
}

// external access API
void writeFT5336DeviceMode(uint8_t data)
{
	writeFTReg(FT5336_OP_DEVICE_MODE, data);
}

uint8_t readFT5336DeviceMode(void)
{
	return readFTReg(FT5336_OP_DEVICE_MODE);
}

uint8_t readFT5336GestureID(void)
{
	return readFTReg(FT5336_OP_GEST_ID);
}

uint8_t readFT5336PointNum(void)
{
	uint8_t number = readFTReg(FT5336_OP_TD_STATUS);
	return number & 0x0F;
}

uint8_t readFT5336EventFlag(void)
{
	uint8_t touch1XH = readFTReg(FT5336_OP_TOUCH1_XH);
	return (touch1XH & 0xC0) >> 6;
}

uint16_t readFT5336PointX(void)
{
	uint8_t touch1XH = readFTReg(FT5336_OP_TOUCH1_XH);
	uint8_t touch1XL = readFTReg(FT5336_OP_TOUCH1_XL);
	
	return (uint16_t)(((touch1XH & 0x0F) << 8) | touch1XL);
}

uint16_t readFT5336PointY(void)
{
	uint8_t touch1YH = readFTReg(FT5336_OP_TOUCH1_YH);
	uint8_t touch1YL = readFTReg(FT5336_OP_TOUCH1_YL);
	
	return (uint16_t)(((touch1YH & 0x0F) << 8) | touch1YL);
}

void enableFT5336Int(void)
{
	// Description in FT5336 data-sheet is error.
	// Switching interrupt mode need setting this register to 1.
	writeFTReg(FT5336_OP_G_MODE, 0x01);
}

void disableFT5336Int(void)
{
	writeFTReg(FT5336_OP_G_MODE, 0x00);
}

uint8_t readFT5336GMode(void)
{
	return readFTReg(FT5336_OP_G_MODE);
}

uint8_t readFT5336ChipID(void)
{
	return readFTReg(FT5336_OP_G_FT5210ID);
}
