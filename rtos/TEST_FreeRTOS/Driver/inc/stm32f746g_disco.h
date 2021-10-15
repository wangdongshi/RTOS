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

#define SD_BLOCK_SIZE			(512)
#define SD_BLOCK_NUM			(1000000000 / SD_BLOCK_SIZE)

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

typedef enum {
	SD_OP_INITIAL = 0,
	SD_OP_ENUMERATE,
	SD_OP_IDLE,
	SD_OP_SINGLE_BLOCK_READ,
	SD_OP_MULTI_BLOCK_READ,
	SD_OP_SINGLE_BLOCK_WRITE,
	SD_OP_MULTI_BLOCK_WRITE
} SD_OP_STATUS;

typedef enum {
	SD_RESPONSE_R0	= 0,
	SD_RESPONSE_R1	= 1,
	SD_RESPONSE_R2	= 2,
	SD_RESPONSE_R3	= 3,
	SD_RESPONSE_R6	= 6,
	SD_RESPONSE_R7	= 7
} SD_RESP_TYPE;

typedef enum {
	// SD Card normal commands
	SD_CMD_GO_IDLE_STATE			= 0,
	SD_CMD_SEND_OP_COND				= 1,
	SD_CMD_ALL_SEND_CID				= 2,
	SD_CMD_SET_REL_ADDR				= 3,
	SD_CMD_SET_DSR					= 4,
	SD_CMD_SDMMC_SEN_OP_COND		= 5,
	SD_CMD_HS_SWITCH				= 6,
	SD_CMD_SEL_DESEL_CARD			= 7,
	SD_CMD_HS_SEND_EXT_CSD			= 8,
	SD_CMD_SEND_CSD					= 9,
	SD_CMD_SEND_CID					= 10,
	SD_CMD_READ_DAT_UNTIL_STOP		= 11,
	SD_CMD_STOP_TRANSMISSION		= 12,
	SD_CMD_SEND_STATUS				= 13,
	SD_CMD_HS_BUSTEST_READ			= 14,
	SD_CMD_GO_INACTIVE_STATE		= 15,
	SD_CMD_SET_BLOCKLEN				= 16,
	SD_CMD_READ_SINGLE_BLOCK		= 17,
	SD_CMD_READ_MULT_BLOCK			= 18,
	SD_CMD_HS_BUSTEST_WRITE			= 19,
	SD_CMD_WRITE_DAT_UNTIL_STOP		= 20,
	SD_CMD_SET_BLOCK_COUNT			= 23,
	SD_CMD_WRITE_SINGLE_BLOCK		= 24,
	SD_CMD_WRITE_MULT_BLOCK			= 25,
	SD_CMD_PROG_CID					= 26,
	SD_CMD_PROG_CSD					= 27,
	SD_CMD_SET_WRITE_PROT			= 28,
	SD_CMD_CLR_WRITE_PROT			= 29,
	SD_CMD_SEND_WRITE_PROT			= 30,
	SD_CMD_SD_ERASE_GRP_START		= 32,
	SD_CMD_SD_ERASE_GRP_END			= 33,
	SD_CMD_ERASE_GRP_START			= 35,
	SD_CMD_ERASE_GRP_END			= 36,
	SD_CMD_ERASE					= 38,
	SD_CMD_FAST_IO					= 39,
	SD_CMD_GO_IRQ_STATE				= 40,
	SD_CMD_LOCK_UNLOCK				= 42,
	SD_CMD_APP_CMD					= 55,
	SD_CMD_GEN_CMD					= 56,
	SD_CMD_NO_CMD					= 64,
	// SD Card specific commands
	SD_CMD_APP_SET_BUSWIDTH			= 6,
	SD_CMD_APP_STATUS				= 13,
	SD_CMD_APP_SEND_NUM_WRITE_BLOCK = 22,
	SD_CMD_APP_OP_COND				= 41,
	SD_CMD_APP_SET_CLR_CARD_DETECT	= 42,
	SD_CMD_APP_SEND_SCR				= 51,
	SD_CMD_SDMMC_RW_DIRECT			= 52,
	SD_CMD_SDMMC_RW_EXTENDED		= 53
} SD_COMMAND;

// Defined in link script
extern uint32_t _ssdram; // SDRAM start address
extern uint32_t _sdram_size;

// Refer other global parameter
extern volatile SD_OP_STATUS sdOpStatus;

void SystemInit(void);

uint8_t usart1ReceiveChar(void);
void usart1SendChar(const uint8_t character);
void usart1Send(const uint8_t* message);

void i2c3Write1Byte(const uint8_t slaveAddr, const uint8_t devAddr, const uint8_t data);
uint8_t i2c3Read1Byte(const uint8_t slaveAddr, const uint8_t devAddr);

bool_t isSDCardInsert(void);
bool_t sdmmcSendCmd(const SD_COMMAND cmd, const SD_RESP_TYPE resp, const uint32_t arg);
bool_t sdPollingRead(const uint32_t blockAddr, const uint32_t blockNum, uint8_t* buf);
bool_t sdPollingWrite(const uint32_t blockAddr, const uint32_t blockNum, uint8_t* buf);
bool_t sdDMARead(const uint32_t blockAddr, const uint32_t blockNum, uint8_t* buf);
bool_t sdDMAWrite(const uint32_t blockAddr, const uint32_t blockNum, uint8_t* buf);

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
bool_t setSDCardData(const uint16_t blockAddr);
bool_t getSDCardData(const uint16_t blockAddr);
bool_t checkSDCardData(void);

#ifdef __cplusplus
}
#endif

#endif // __STM32F746G_DISCO_H__
