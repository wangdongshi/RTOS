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

// SD constant value definition
#define SD_BLOCKSIZE					(512)
//#define SD_SEND_CMD_TIMEOUT_CNT			(5000 * (216000000 / 8 / 1000))
//#define SD_STOP_TRANS_TIMEOUT_CNT		(100000000 * (216000000 / 8 / 1000))
//#define SD_CMD_RESP_R1_ERRORBITS		(0xFDFFE008)
//#define SD_CMD_RESP_R6_ERRORBITS		(0x0000E000)
#define SD_RESPONSE_R0					(0)
#define SD_RESPONSE_R1					(1)
#define SD_RESPONSE_R2					(2)
#define SD_RESPONSE_R3					(3)
#define SD_RESPONSE_R6					(6)
#define SD_RESPONSE_R7					(7)
// SD Card normal commands
#define SD_CMD_GO_IDLE_STATE			(0)
#define SD_CMD_SEND_OP_COND				(1)
#define SD_CMD_ALL_SEND_CID				(2)
#define SD_CMD_SET_REL_ADDR				(3)
#define SD_CMD_SET_DSR					(4)
#define SD_CMD_SDMMC_SEN_OP_COND		(5)
#define SD_CMD_HS_SWITCH				(6)
#define SD_CMD_SEL_DESEL_CARD			(7)
#define SD_CMD_HS_SEND_EXT_CSD			(8)
#define SD_CMD_SEND_CSD					(9)
#define SD_CMD_SEND_CID					(10)
#define SD_CMD_READ_DAT_UNTIL_STOP		(11)
#define SD_CMD_STOP_TRANSMISSION		(12)
#define SD_CMD_SEND_STATUS				(13)
#define SD_CMD_HS_BUSTEST_READ			(14)
#define SD_CMD_GO_INACTIVE_STATE		(15)
#define SD_CMD_SET_BLOCKLEN				(16)
#define SD_CMD_READ_SINGLE_BLOCK		(17)
#define SD_CMD_READ_MULT_BLOCK			(18)
#define SD_CMD_HS_BUSTEST_WRITE			(19)
#define SD_CMD_WRITE_DAT_UNTIL_STOP		(20)
#define SD_CMD_SET_BLOCK_COUNT			(23)
#define SD_CMD_WRITE_SINGLE_BLOCK		(24)
#define SD_CMD_WRITE_MULT_BLOCK			(25)
#define SD_CMD_PROG_CID					(26)
#define SD_CMD_PROG_CSD					(27)
#define SD_CMD_SET_WRITE_PROT			(28)
#define SD_CMD_CLR_WRITE_PROT			(29)
#define SD_CMD_SEND_WRITE_PROT			(30)
#define SD_CMD_SD_ERASE_GRP_START		(32)
#define SD_CMD_SD_ERASE_GRP_END			(33)
#define SD_CMD_ERASE_GRP_START			(35)
#define SD_CMD_ERASE_GRP_END			(36)
#define SD_CMD_ERASE					(38)
#define SD_CMD_FAST_IO					(39)
#define SD_CMD_GO_IRQ_STATE				(40)
#define SD_CMD_LOCK_UNLOCK				(42)
#define SD_CMD_APP_CMD					(55)
#define SD_CMD_GEN_CMD					(56)
#define SD_CMD_NO_CMD					(64)
// SD Card specific commands
#define SD_CMD_APP_SET_BUSWIDTH			(6)
#define SD_CMD_APP_STATUS				(13)
#define SD_CMD_APP_SEND_NUM_WRITE_BLOCK (22)
#define SD_CMD_APP_OP_COND				(41)
#define SD_CMD_APP_SET_CLR_CARD_DETECT	(42)
#define SD_CMD_APP_SEND_SCR				(51)
#define SD_CMD_SDMMC_RW_DIRECT			(52)
#define SD_CMD_SDMMC_RW_EXTENDED		(53)

// Defined in link script
extern uint32_t _ssdram; // SDRAM start address
extern uint32_t _sdram_size;

void SystemInit(void);

uint8_t usart1ReceiveChar(void);
void usart1SendChar(const uint8_t character);
void usart1Send(const uint8_t* message);

void i2c3Write1Byte(const uint8_t slaveAddr, const uint8_t devAddr, const uint8_t data);
uint8_t i2c3Read1Byte(const uint8_t slaveAddr, const uint8_t devAddr);

bool_t isSDCardInsert(void);
bool_t sdmmcSendCmd(const uint8_t cmd, const uint8_t resp, const uint32_t arg);
bool_t sdRead1Block(const uint32_t blockAddr, uint8_t* buf);
bool_t sdWrite1Block(const uint32_t blockAddr, uint8_t* buf);
bool_t sdReadDMA(const uint32_t blockAddr, const uint32_t blockNum, uint8_t* buf);

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
bool_t checkSDMMC(const uint16_t data);

#ifdef __cplusplus
}
#endif

#endif // __STM32F746G_DISCO_H__
