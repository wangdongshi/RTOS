/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : stm32f746g_disco.c
 * Project  : Minimum RTOS platform
 * Date     : 2021/9/5
 * Author   : WangYu
 *
 **********************************************************************/
#include <string.h>
#include "platform.h"
#include "types.h"
#include "debug.h"
#include "assert_param.h"
#include "font.h"
#include "image.h"
#include "stm32f746xx.h"
#include "stm32f746g_disco.h"
#include "ft5336.h"

// Clock constant value definition
#define PLLM							((uint32_t)( 25 <<  0))
#define PLLN							((uint32_t)(432 <<  6))
#define PLLP							((uint32_t)(  0 << 16)) // pay attention to this value!!
#define PLLSRC							((uint32_t)(  1 << 22))
#define PLLQ							((uint32_t)(  9 << 24))

// LCD constant value definition
#define COLOR_BYTE_ARGB8888				(4)
#define COLOR_BYTE_RGB888				(3)
#define LCD_FRAME_BUF_BYTES				COLOR_BYTE_ARGB8888
#define LCD_FRAME_BUF_SIZE				(LCD_FRAME_BUF_BYTES * LCD_ACTIVE_WIDTH * LCD_ACTIVE_HEIGHT)
#define LCD_HSYNC						(41)
#define LCD_VSYNC						(10)
#define LCD_HBP							(13)
#define LCD_HFP							(32)
#define LCD_VBP							(2)
#define LCD_VFP							(2)

// SD constant value definition
#define SD_BLOCKSIZE					(512)
#define SD_TIMEOUT_CNT_BASE				(216000000 / 8 / 1000)
#define SD_SEND_CMD_TIMEOUT_CNT			(5000 * SD_TIMEOUT_CNT_BASE)
#define SD_STOP_TRANS_TIMEOUT_CNT		(10000 * SD_TIMEOUT_CNT_BASE)
#define SD_CMD_RESP_R1_ERRORBITS		(0xFDFFE008)
#define SD_CMD_RESP_R6_ERRORBITS		(0x0000E000)
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

typedef enum {
	SD_VERSION_1X,
	SD_VERSION_2X
} SD_VERSION;

typedef enum {
	SD_TYPE_SDSC,
	SD_TYPE_SDHC_SDXC
} SD_TYPE;

typedef struct {
	uint32_t	raw[4];
	uint32_t	others[0];
} SD_CID;

typedef struct {
	uint32_t	raw[4];
	uint32_t	others[0];
} SD_CSD;

typedef struct {
	uint32_t	raw[2];
	uint8_t		wideBus;
	uint32_t	others[0];
} SD_SCR;

typedef struct {
	SD_VERSION	version;
	SD_TYPE		type;
	uint32_t	category;
	uint16_t	rca;
	SD_CID		cid;
	SD_CSD		csd;
	SD_SCR		scr;
} SD_INFO;

SD_INFO __attribute__( ( aligned(4) ) ) sdcard;

uint8_t __attribute__( ( section(".sdram" ) ) ) __attribute__( ( aligned(4) ) )   charBuffer[12 * 16 * COLOR_BYTE_ARGB8888];
uint8_t __attribute__( ( section(".sdram" ) ) ) __attribute__( ( aligned(4) ) ) frameBuffer1[LCD_FRAME_BUF_SIZE];
uint8_t __attribute__( ( section(".sdram" ) ) ) __attribute__( ( aligned(4) ) ) frameBuffer2[LCD_FRAME_BUF_SIZE];

static void initFPU(void);
static void initSystemClock(void);
static void initSDRAM(void);
static void initSystick(void);
static void initUSART1(void);
static void initTouchPanel(void);
static void initLCD(void);
static void initDMA2D(void);
static void initLED1(void);
static void initSDMMC(void);
static bool_t initSDCard(void);
#ifdef MODE_STAND_ALONE
static void initTIM7(void);
#endif
static void initNVICPriority(void);

static void initSDRAMGPIO(void);
static void initUartGPIO(void);
static void initLED1GPIO(void);
static void initLCDGPIO(void);
static void initTouchPanelGPIO(void);
static void initSDMMCGPIO(void);

static void setCharBuf06x08(
		const uint16_t  symbol,
		const uint32_t foreColor,
		const uint32_t backColor);
static void setCharBuf12x16(
		const uint16_t  symbol,
		const uint32_t foreColor,
		const uint32_t backColor);

static bool_t sdmmcCheckCmdResp0(void);
static bool_t sdmmcCheckCmdResp1(uint8_t cmd);
static bool_t sdmmcCheckCmdResp2(uint8_t cmd);
static bool_t sdmmcCheckCmdResp3(void);
static bool_t sdmmcCheckCmdResp6(uint8_t cmd);
static bool_t sdmmcCheckCmdResp7(void);

// External API function group
void SystemInit(void)
{
	SCB_EnableICache();
	SCB_EnableDCache();
	initFPU();
	initSystemClock();
	initSDRAM();
	initNVICPriority();
	initUSART1();
	TRACE("STM32F746G-DISCO starts to power up...\r\n");
	initLED1();
	initLCD();
	TRACE("1. LCD initialization is success.\r\n");
	initTouchPanel();
	TRACE("2. Touch panel initialization is success.\r\n");
	initSDMMC();
	char* msg[2] = {
		"3. SD card is not inserted.\r\n",
		"3. SD card identification is success.\r\n"
	};
	TRACE(msg[initSDCard()]);
	initSystick();
#ifdef MODE_STAND_ALONE
	initTIM7();
#endif
	TRACE("\r\n");
}

void usart1SendChar(const uint8_t character)
{
	while ((USART1->ISR & USART_ISR_TC_Msk) == 0);
	USART1->TDR = (uint32_t)character;
}

uint8_t usart1ReceiveChar(void)
{
	while ((USART1->ISR & USART_ISR_RXNE_Msk) == 0);
	return (uint8_t)(USART1->RDR & 0xFF);
}

void usart1Send(const uint8_t* message)
{
	USART1->CR1	|= USART_CR1_TE;
	for(uint32_t i = 0; i < strlen((const char*)message); i++) {
		USART1->TDR = message[i];
		while ((USART1->ISR & USART_ISR_TXE_Msk) == 0);
	}
	while ((USART1->ISR & USART_ISR_TC_Msk) == 0);
	USART1->CR1	&= ~USART_CR1_TE;
}


// for 7bits slave address and 8bits memory address
void i2c3Write1Byte(const uint8_t slaveAddr, const uint8_t devAddr, const uint8_t data)
{
	// Wait previous I2C operation complete
	while (I2C3->ISR & I2C_ISR_BUSY);

	// Set master communication to write mode for send device address and data
	I2C3->CR2	=	I2C_CR2_AUTOEND |
					2 << I2C_CR2_NBYTES_Pos |	// send 2 bytes (address + data)
					I2C_CR2_START |
					0b0 << I2C_CR2_RD_WRN_Pos |	// write
					slaveAddr << I2C_CR2_SADD_Pos;

	// Send device address and data
	while ((I2C3->ISR & I2C_ISR_TXIS_Msk) == 0);
	I2C3->TXDR	=	devAddr;
	while ((I2C3->ISR & I2C_ISR_TXIS_Msk) == 0);
	I2C3->TXDR	=	data;
	// No need to Check TC flag, with AUTOEND mode the stop is automatically generated
	//while ((I2C3->ISR & I2C_ISR_TC_Msk) == 0);
	while ((I2C3->ISR & I2C_ISR_STOPF_Msk) == 0);
	I2C3->ICR	|=	I2C_ICR_STOPCF;
}

// for 7bits slave address and 8bits memory address
uint8_t i2c3Read1Byte(const uint8_t slaveAddr, const uint8_t devAddr)
{
	uint8_t data;

	// Wait previous I2C operation complete
	while (I2C3->ISR & I2C_ISR_BUSY_Msk);

	// Set master communication to write mode for send device address
	I2C3->CR2	=	1 << I2C_CR2_NBYTES_Pos |
					I2C_CR2_START |
					0b0 << I2C_CR2_RD_WRN_Pos | // write
					slaveAddr << I2C_CR2_SADD_Pos;
	while ((I2C3->ISR & I2C_ISR_TXIS_Msk) == 0);
	I2C3->TXDR	=	devAddr;
	while ((I2C3->ISR & I2C_ISR_TC_Msk) == 0);

	// Set master communication to read mode for receive data
	I2C3->CR2	=	I2C_CR2_AUTOEND |
					1 << I2C_CR2_NBYTES_Pos |
					I2C_CR2_START |
					0b1 << I2C_CR2_RD_WRN_Pos | // read
					slaveAddr << I2C_CR2_SADD_Pos;
	while ((I2C3->ISR & I2C_ISR_RXNE_Msk) == 0);
	data		=	(uint8_t)I2C3->RXDR;
	while ((I2C3->ISR & I2C_ISR_STOPF_Msk) == 0);
	I2C3->ICR	|=	I2C_ICR_STOPCF;

	return data;
}

void toggleLED1(void)
{
	uint32_t data = GPIOI->ODR & GPIO_ODR_OD1_Msk;
	GPIOI->ODR &= ~GPIO_ODR_OD1_Msk;
	GPIOI->ODR |= (~data) & GPIO_ODR_OD1_Msk;
}

bool_t isSDCardInsert(void)
{
	bool_t res = !(bool_t)((GPIOC->IDR & GPIO_IDR_ID13_Msk) >> GPIO_IDR_ID13_Pos);
	return res;
}

bool_t sdmmcSendCmd(const uint8_t cmd, const uint8_t resp, const uint32_t arg)
{
	uint8_t respLength = 0b01;

	// set WAITRESP area
	switch (cmd) {
	case SD_CMD_GO_IDLE_STATE:
		respLength = 0b00; // no response
		break;
	case SD_CMD_ALL_SEND_CID:
	case SD_CMD_SEND_CSD:
		respLength = 0b11; // long response
		break;
	default:
		respLength = 0b01; // short response
		break;
	}

	// send command
	SDMMC1->ARG	=	arg;
	SDMMC1->CMD	=	cmd << SDMMC_CMD_CMDINDEX_Pos |
					respLength << SDMMC_CMD_WAITRESP_Pos |
					0b0 << SDMMC_CMD_WAITINT_Pos | // no interrupt
					0b1 << SDMMC_CMD_CPSMEN_Pos;   // enable CPSM

	// check response
	bool_t res = False;
	switch (resp) {
	case SD_RESPONSE_R0:
		res = sdmmcCheckCmdResp0();
		break;
	default:
	case SD_RESPONSE_R1:
		res = sdmmcCheckCmdResp1(cmd);
		break;
	case SD_RESPONSE_R2:
		res = sdmmcCheckCmdResp2(cmd);
		break;
	case SD_RESPONSE_R3:
		res = sdmmcCheckCmdResp3();
		break;
	case SD_RESPONSE_R6:
		res = sdmmcCheckCmdResp6(cmd);
		break;
	case SD_RESPONSE_R7:
		res = sdmmcCheckCmdResp7();
		break;
	}

	return res;
}

bool_t sdReadDMA(const uint32_t blockAddr, const uint32_t blockNum, uint8_t* buf)
{
	// 1. Send CMD16 to set card block size.
	//if (!sdmmcSendCmd(SD_CMD_SET_BLOCKLEN, SD_RESPONSE_R1, SD_BLOCKSIZE)) return False;

	// 2. Clear DCTRL register
	// According to the notes of 35.8.9 chapter of STM32F746XX RM,
	// the DCTRL register setting must follow this conditions:
	// >>>> After a data write, data cannot be written to this register for
	// >>>> three SDMMCCLK (48 MHz)	clock periods plus two PCLK2 clock periods.
	// Note: PCLK2 is 108MHz in this system.
	// According test result, here must insert the delay as below !!!
	for (volatile uint32_t i = 0; i < 500000; i++);
	SDMMC1->DCTRL = 0;
	SDMMC1->ICR = SDMMC1->STA;

	// 3. Enable SDMMC interrupt.
	SDMMC1->MASK	|=	SDMMC_MASK_CCRCFAILIE_Msk |
						SDMMC_MASK_DTIMEOUTIE_Msk |
						SDMMC_MASK_RXOVERRIE_Msk |
						SDMMC_MASK_DATAENDIE_Msk;

	// 4. Connect DMA channel (stream3/channel4) between SDMMC and memory.
	// Note : DMA2_Stream3->CR setting has been execute in device initialization phase.
	DMA2_Stream3->NDTR = (uint32_t)(blockNum * SD_BLOCKSIZE / 4); // by word (DMA2_Stream3 transfer unit)
	DMA2_Stream3->PAR  = (uint32_t)(&(SDMMC1->FIFO));
	DMA2_Stream3->M0AR = (uint32_t)(buf);
	DMA2_Stream3->CR |= DMA_SxCR_EN; // start DMA transfer

	// 5. Configure SDMMC data transfer mode and enable DMA stream.
	SDMMC1->DTIMER	=	0xFFFFFFFF;
	SDMMC1->DLEN	=	blockNum * SD_BLOCKSIZE;				// data length
	SDMMC1->DCTRL	=	0b1001 << SDMMC_DCTRL_DBLOCKSIZE_Pos |	// 512 Bytes
						0b1 << SDMMC_DCTRL_DTDIR_Pos |			// card --> SDMMC
						0b0 << SDMMC_DCTRL_DTMODE_Pos |			// block mode
						0b1 << SDMMC_DCTRL_DMAEN_Pos |			// enable DMA
						0b1 << SDMMC_DCTRL_DTEN_Pos;			// enable data transfer

	// 6. Send CMD17 or CMD18 to notify SD card send data.
	uint32_t addr = blockAddr * SD_BLOCKSIZE;
	if (blockNum == 1) {
		if (!sdmmcSendCmd(SD_CMD_READ_SINGLE_BLOCK, SD_RESPONSE_R1, addr)) return False;
	}
	else {
		if (!sdmmcSendCmd(SD_CMD_READ_MULT_BLOCK, SD_RESPONSE_R1, addr)) return False;
	}

	return True;
}

bool_t sdRead1Block(const uint32_t blockAddr, uint8_t* buf)
{
	// According to the notes of 35.8.9 chapter of STM32F746XX RM,
	// the DCTRL register setting must follow this conditions:
	// >>>> After a data write, data cannot be written to this register for
	// >>>> three SDMMCCLK (48 MHz)	clock periods plus two PCLK2 clock periods.
	// Note: PCLK2 is 108MHz in this system.
	// According test result, here must insert the delay as below !!!
	for (volatile uint32_t i = 0; i < 500000; i++);

	// Configure SDMMC to receive mode.
	SDMMC1->DTIMER	=	0xFFFFFFFF;
	SDMMC1->DLEN	=	SD_BLOCKSIZE;	// 512 Bytes
	SDMMC1->DCTRL	=	0b1001 << SDMMC_DCTRL_DBLOCKSIZE_Pos |	// 512 Bytes
						0b1 << SDMMC_DCTRL_DTDIR_Pos |			// card --> SDMMC
						0b0 << SDMMC_DCTRL_DTMODE_Pos |			// block mode
						0b1 << SDMMC_DCTRL_DTEN_Pos;			// enable data transfer

	// Send CMD17 to read one block from SD card.
	uint32_t addr = blockAddr * SD_BLOCKSIZE;
	if (!sdmmcSendCmd(SD_CMD_READ_SINGLE_BLOCK, SD_RESPONSE_R1, addr)) return False;

	// Poll reading data from SDMMC RX FIFO.
	uint32_t errMask = SDMMC_STA_RXOVERR_Msk | SDMMC_STA_DCRCFAIL_Msk | SDMMC_STA_DTIMEOUT_Msk;
	uint32_t remain  = SD_BLOCKSIZE;
	while (remain > 0) {
		if (SDMMC1->STA & errMask) return False;
		if (SDMMC1->STA & SDMMC_STA_RXDAVL_Msk) {
			uint32_t data = SDMMC1->FIFO;
			*buf++ = (uint8_t)((data >> 0 ) & 0xFF);
			*buf++ = (uint8_t)((data >> 8 ) & 0xFF);
			*buf++ = (uint8_t)((data >> 16) & 0xFF);
			*buf++ = (uint8_t)((data >> 24) & 0xFF);
			remain -= 4;
		}
	}
	SDMMC1->ICR = SDMMC1->STA;

	return True;
}

bool_t sdWrite1Block(const uint32_t blockAddr, uint8_t* buf)
{
	// According to the notes of 35.8.9 chapter of STM32F746XX RM,
	// the DCTRL register setting must follow this conditions:
	// >>>> After a data write, data cannot be written to this register for
	// >>>> three SDMMCCLK (48 MHz)	clock periods plus two PCLK2 clock periods.
	// Note: PCLK2 is 108MHz in this system.
	// According test result, here must insert the delay as below !!!
	for (volatile uint32_t i = 0; i < 500000; i++);

	// Configure SDMMC to receive mode
	SDMMC1->DTIMER	=	0xFFFFFFFF;
	SDMMC1->DLEN	=	SD_BLOCKSIZE;	// 512 Bytes
	SDMMC1->DCTRL	=	0b1001 << SDMMC_DCTRL_DBLOCKSIZE_Pos |	// 512 Bytes
						0b0 << SDMMC_DCTRL_DTDIR_Pos |			// SDMMC --> card
						0b0 << SDMMC_DCTRL_DTMODE_Pos |			// block mode
						0b1 << SDMMC_DCTRL_DTEN_Pos;			// enable data transfer

	// Send CMD24 to write one block to SD card.
	uint32_t addr = blockAddr * SD_BLOCKSIZE;
	if (!sdmmcSendCmd(SD_CMD_WRITE_SINGLE_BLOCK, SD_RESPONSE_R1, addr)) return False;

	// Poll writing data to SDMMC TX FIFO
	uint32_t remain = SD_BLOCKSIZE;
	uint32_t errMask = SDMMC_STA_TXUNDERR_Msk | SDMMC_STA_DCRCFAIL_Msk | SDMMC_STA_DTIMEOUT_Msk;
	while (remain > 0) {
		if (SDMMC1->STA & errMask) return False;
		if (SDMMC1->STA & SDMMC_STA_TXFIFOHE_Msk) {
			for (uint8_t count = 0; count < 8; count++) {
				uint32_t data = 0;
				data |= (uint32_t)(*buf++) << 0;
				data |= (uint32_t)(*buf++) << 8;
				data |= (uint32_t)(*buf++) << 16;
				data |= (uint32_t)(*buf++) << 24;
				remain -= 4;
				SDMMC1->FIFO = data;
			}
		}
	}
	SDMMC1->ICR = SDMMC1->STA;

	return True;
}

void showLogo(void)
{
	drawImage(0, 0, 480, 272, (uint32_t)&logoImage, COLOR_RGB888, LAYER_FG);
}

void checkDMA2D(void)
{
	fillRect(0,  0,  480, 272, 0x00000000, LAYER_FG); // clear logo
	fillRect(0,  0,  480, 272, 0xFFA9A9A9, LAYER_BG);
	fillRect(50, 50, 380, 132, 0xFF8B0000, LAYER_BG);
	drawString(70,  60,  0xFFA9A9A9, 0x008B0000, "Welcome to STM32F746G-DISCO!", FONT_MIDDLE, LAYER_FG);
	drawString(280, 120, 0xFFA9A9A9, 0x008B0000, "Author : Wang.Yu", FONT_SMALL, LAYER_FG);
	drawString(280, 150, 0xFFA9A9A9, 0x008B0000, "  Date : 2021/9/1", FONT_SMALL, LAYER_FG);
#if 0
	drawString(70,  80,  0xFFA9A9A9, 0x008B0000, "abcdefghijklmnopqrstuvwxyz", FONT_SMALL, LAYER_FG);
	drawString(70,  100, 0xFFA9A9A9, 0x008B0000, "abcdefghijklmnopqrstuvwxyz", FONT_MIDDLE, LAYER_FG);
#endif
}

bool_t checkSDRAM(void)
{
	uint8_t* p;
	uint8_t* startAddr	= (uint8_t*)&_ssdram;
	uint8_t* endAddr	= (uint8_t*)((uint32_t)&_ssdram + (uint32_t)&_sdram_size - 1);

	// check after write
	for (p = startAddr; p <= endAddr; p += 0x100000) {
		*p = 0xA5;
		if ((*p) != 0xA5) return False;
	}
	*endAddr = 0x5A;
	if ((*endAddr) != 0x5A) return False;

	// check delay
	for (volatile uint32_t i = 0; i < 1000000; i++);
	for (p = startAddr; p <= endAddr; p += 0x100000) {
		if ((*p) != 0xA5) return False;
	}
	if ((*endAddr) != 0x5A) return False;

	return True;
}

bool_t checkTouchPanel(void)
{
	uint8_t chipID = readFT5336ChipID();
	return (chipID == 0x51);
}

void fillRect(
		const uint16_t x,
		const uint16_t y,
		const uint16_t w,
		const uint16_t h,
		const uint32_t color,
		const LAYER_TYPE layer)
{
	uint8_t* pFB = (uint8_t*)((layer == LAYER_BG) ? &frameBuffer1 : &frameBuffer2);
	while (DMA2D->CR & DMA2D_CR_START); // wait previous transfer complete

	DMA2D->CR 		&= ~DMA2D_CR_MODE_Msk;
	DMA2D->CR 		|= 0b11 << DMA2D_CR_MODE_Pos; // register to memory
	DMA2D->OCOLR	= color;
	DMA2D->OMAR		= (uint32_t)(&(pFB[(y * LCD_ACTIVE_WIDTH + x) * LCD_FRAME_BUF_BYTES]));
	DMA2D->OOR		= LCD_ACTIVE_WIDTH - w;
	DMA2D->OPFCCR	= 0b000; // ARGB8888
	DMA2D->NLR		= w << DMA2D_NLR_PL_Pos | h << DMA2D_NLR_NL_Pos;

	DMA2D->CR   	|= DMA2D_CR_START;
}

void drawImage(
		const uint16_t x,
		const uint16_t y,
		const uint16_t w,
		const uint16_t h,
		const uint32_t addr,
		const RGB_TYPE pfc,
		const LAYER_TYPE layer)
{
	uint8_t* pFB = (uint8_t*)((layer == LAYER_BG) ? &frameBuffer1 : &frameBuffer2);
	while (DMA2D->CR & DMA2D_CR_START); // wait previous transfer complete

	DMA2D->CR 		&= ~DMA2D_CR_MODE_Msk;
	DMA2D->CR 		|= 0b01 << DMA2D_CR_MODE_Pos; // FG PFC
	DMA2D->FGMAR	= addr;
	DMA2D->OMAR		= (uint32_t)(&(pFB[(y * LCD_ACTIVE_WIDTH + x) * LCD_FRAME_BUF_BYTES]));
	DMA2D->FGOR		= 0;
	DMA2D->OOR		= LCD_ACTIVE_WIDTH - w;
	DMA2D->FGPFCCR	= pfc;   // fore-ground RGB format
	DMA2D->OPFCCR	= 0b000; // ARGB8888
	DMA2D->NLR		= w << DMA2D_NLR_PL_Pos | h << DMA2D_NLR_NL_Pos;

	DMA2D->CR   	|= DMA2D_CR_START;
}

void drawChar(
		const uint16_t x,
		const uint16_t y,
		const uint32_t foreColor,
		const uint32_t backColor,
		const uint16_t symbol,
		const FONT_TYPE fontType,
		const LAYER_TYPE layer)
{
	uint32_t height = (FONT_SMALL == fontType) ? sFont.st.head.fontHeight    : mFont.st.head.fontHeight;
	uint32_t width  = (FONT_SMALL == fontType) ? sFont.st.cdef[symbol].width : mFont.st.cdef[symbol].width;
	uint8_t* pFB = (uint8_t*)((layer == LAYER_BG) ? &frameBuffer1 : &frameBuffer2);

	while (DMA2D->CR & DMA2D_CR_START); // wait previous transfer complete

	if (FONT_SMALL == fontType) {
		setCharBuf06x08(symbol, foreColor, backColor);
	}
	else {
		setCharBuf12x16(symbol, foreColor, backColor);
	}

	DMA2D->CR 		&= ~DMA2D_CR_MODE_Msk;
	DMA2D->CR 		|= 0b01 << DMA2D_CR_MODE_Pos; // FG PFC
	DMA2D->FGMAR	= (uint32_t)&charBuffer;
	DMA2D->OMAR		= (uint32_t)(&(pFB[(y * LCD_ACTIVE_WIDTH + x) * LCD_FRAME_BUF_BYTES]));
	DMA2D->FGOR		= 0;
	DMA2D->OOR		= LCD_ACTIVE_WIDTH - width;
	DMA2D->FGPFCCR	= 0b0000; // ARGB8888
	DMA2D->OPFCCR	= 0b000;  // ARGB8888
	DMA2D->NLR		= width << DMA2D_NLR_PL_Pos | height << DMA2D_NLR_NL_Pos;

	DMA2D->CR   	|= DMA2D_CR_START;
}

void drawString( // It only can support 1 line string
		const uint16_t x,
		const uint16_t y,
		const uint32_t foreColor,
		const uint32_t backColor,
		const char* const string,
		const FONT_TYPE fontType,
		const LAYER_TYPE layer)
{
	uint16_t currentX = x;
	char* pString = (char*)string;
	while (*pString != '\0') {
		drawChar(currentX, y, foreColor, backColor, *pString, fontType, LAYER_FG);
		currentX += (FONT_SMALL == fontType) ? sFont.st.cdef[(uint32_t)(*pString)].width : mFont.st.cdef[(uint32_t)(*pString)].width;
		pString++;
	}
}

uint32_t getRandomData(void)
{
	RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;
	while ((RCC->AHB2ENR & RCC_AHB2ENR_RNGEN_Msk) == 0);

	RNG->CR = RNG_CR_RNGEN;
	while ((RNG->SR & RNG_SR_DRDY_Msk) == 0);

	uint32_t random = RNG->DR;

	RCC->AHB2RSTR |= RCC_AHB2RSTR_RNGRST;
	return random;
}

bool_t checkDMA(uint16_t data)
{
	uint32_t  size = 0x8000;
	uint16_t* pSrc = &data;
	uint16_t* pDes = (uint16_t*)((uint32_t)&_ssdram + (uint32_t)(0x500000/sizeof(uint16_t)));

	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	while ((RCC->AHB1ENR & RCC_AHB1ENR_DMA2EN_Msk) == 0);

	DMA2_Stream0->CR = 	DMA_SxCR_CHSEL_0 |	// Stream0, channel1
						DMA_SxCR_PSIZE_0 |	// src data 16bits
						DMA_SxCR_MSIZE_0 |	// des data 16bits
						DMA_SxCR_MINC | 	// des address automatic increase
						DMA_SxCR_PL_0 | DMA_SxCR_PL_1 | // highest DMA priority
						DMA_SxCR_DIR_1; 	// memory to memory
	DMA2_Stream0->NDTR = size/sizeof(uint16_t);	 // 32768 / 2 = 16 K half word
	DMA2_Stream0->PAR  = (uint32_t)(pSrc);
	DMA2_Stream0->M0AR = (uint32_t)(pDes);

	DMA2_Stream0->CR |= DMA_SxCR_EN; // start DMA transfer

	while ((DMA2->LISR & DMA_LISR_TCIF0_Msk) == 0); // wait transfer complete
	DMA2->LIFCR |= DMA_LIFCR_CTCIF0; // must clear TC interrupt flag for next DMA

	//RCC->AHB1RSTR |= RCC_AHB1RSTR_DMA2RST;

	return (*pDes == data && *(pDes + 0x3FF8) == data);
}

bool_t checkSDMMC(const uint16_t data)
{
	uint32_t randomAddr = data * SD_BLOCKSIZE;
	uint8_t srcData[SD_BLOCKSIZE];
	uint8_t desData[SD_BLOCKSIZE*2];

	for (uint16_t i = 0; i < SD_BLOCKSIZE; i++) {
		srcData[i] = (i+2) & 0xFF;
	}
	if (!sdWrite1Block(randomAddr, &srcData[0])) return False;
	if (!sdWrite1Block(randomAddr+SD_BLOCKSIZE, &srcData[0])) return False;
	//if (!sdRead1Block(randomAddr, &desData[0])) return False;
	//if (!sdRead1Block(randomAddr+SD_BLOCKSIZE, &desData[512])) return False;
	if (!sdReadDMA(randomAddr, 2, &desData[0])) return False;
	//vTaskDelay(1000);
	for (volatile uint32_t i = 0; i < 1000000; i++);
	for (uint16_t i = 0; i < SD_BLOCKSIZE; i++) {
		if (srcData[i] != desData[i]) return False;
	}
	return True;
}

// Internal API function group

// The following initialization process is write for STM32F746G-DISCO
// FPU initialization
static void initFPU(void)
{
	SCB->CPACR |= 	0b11 << 20 | // CP10
					0b11 << 22;  // CP11
	while ((SCB->CPACR & (0b1111 << 20)) != (0b1111 << 20));
}

// System clock initialization
static void initSystemClock(void)
{
	// 1. Set HSE and reset RCC_CIR
	RCC->CR |= RCC_CR_HSEBYP | RCC_CR_HSEON;
	while ((RCC->CR & RCC_CR_HSERDY_Msk) == 0);
	RCC->CIR = 0x00000000; // disable all RCC interrupts

	// 2. Set FLASH latency
	FLASH->ACR |= FLASH_ACR_LATENCY_7WS; // need be set in 216MHz
	while ((FLASH->ACR & FLASH_ACR_LATENCY_Msk) != FLASH_ACR_LATENCY_7WS);
	//FLASH->ACR |= FLASH_ACR_ARTEN | FLASH_ACR_PRFTEN;
	//PWR->CR1 |= 0b11 << PWR_CR1_VOS_Pos; // default value has set to 0b11 in reset

	// 3. Enable PWR clock
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	while ((RCC->APB1ENR & RCC_APB1ENR_PWREN_Msk) == 0);

	// 4. Activation OverDrive Mode
	PWR->CR1 |= PWR_CR1_ODEN;
	while ((PWR->CSR1 & PWR_CSR1_ODRDY_Msk) == 0);

	// 5. Activation OverDrive Switching
	PWR->CR1 |= PWR_CR1_ODSWEN;
	while ((PWR->CSR1 & PWR_CSR1_ODSWRDY_Msk) == 0);

	// 6. Main PLL configuration and activation
	RCC->PLLCFGR = PLLM | PLLN | PLLP | PLLSRC | PLLQ;
	RCC->CR |= RCC_CR_PLLON;
	while ((RCC->CR & RCC_CR_PLLRDY_Msk) == 0);

	// 7. System clock activation on the main PLL
	RCC->CFGR |=	RCC_CFGR_HPRE_DIV1 |	 // set HPRE  (AHB  pre-scaler) to 1 (216 MHz)
					RCC_CFGR_PPRE1_DIV4 |	 // set PPRE1 (APB1 pre-scaler) to 4 (54  MHz)
					RCC_CFGR_PPRE2_DIV2;	 // set PPRE2 (APB2 pre-scaler) to 2 (108 MHz)
	// Attention : before switching System clock to PLL, it must wait all pre-scale value set to CFGR.
	while ((RCC->CFGR & (RCC_CFGR_HPRE_Msk | RCC_CFGR_PPRE1_Msk | RCC_CFGR_PPRE2_Msk)) !=
		(RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2));
	RCC->CFGR |=	RCC_CFGR_SW_PLL; // switch main clock to PLL
	while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_PLL);
}

// SDRAM initialization
static void initSDRAM(void)
{
	uint32_t reg, mask;

	// Initialize GPIO for FMC
	initSDRAMGPIO();

	// Enable FMC RCC
	RCC->AHB3ENR	|=	RCC_AHB3ENR_FMCEN;
	while ((RCC->AHB3ENR & RCC_AHB3ENR_FMCEN_Msk) == 0);

	// The following comment is copied from STM32F746G data-sheet FMC-SDRAM chapter.
	// --- Set MCU-FMC register ---
	// 1. Program the memory device features into the FMC_SDCRx register.
	//    The SDRAM clock frequency, RBURST and RPIPE must be programmed in
	//    the FMC_SDCR1 register.
	mask =	FMC_SDCR1_NC_Msk |
			FMC_SDCR1_NR_Msk |
			FMC_SDCR1_MWID_Msk |
			FMC_SDCR1_NB_Msk |
			FMC_SDCR1_CAS_Msk |
			FMC_SDCR1_WP_Msk |
			FMC_SDCR1_SDCLK_Msk |
			FMC_SDCR1_RBURST_Msk |
			FMC_SDCR1_RPIPE_Msk;
	reg  = 	0b00 << FMC_SDCR1_NC_Pos |		// 8 bits
			0b01 << FMC_SDCR1_NR_Pos |		// 12 bits
			0b01 << FMC_SDCR1_MWID_Pos |	// 16 bits
			0b1  << FMC_SDCR1_NB_Pos |		// 4 banks
			0b11 << FMC_SDCR1_CAS_Pos |		// 3 cycles
			0b0  << FMC_SDCR1_WP_Pos |		// enable write operation
			0b10 << FMC_SDCR1_SDCLK_Pos |	// HCLK x 2 = 108MHz = 9.26ns
			0b1  << FMC_SDCR1_RBURST_Pos |	// enable burst mode
			0b00 << FMC_SDCR1_RPIPE_Pos;	// 0 latency after CAS
	FMC_Bank5_6->SDCR[0] &= ~mask;
	FMC_Bank5_6->SDCR[0] |= reg;
	while ((FMC_Bank5_6->SDCR[0] & mask) != reg);

	// 2. Program the memory device timing into the FMC_SDTRx register.
	//    The TRP and TRC timings must be programmed in the FMC_SDTR1 register.
	mask =	FMC_SDTR1_TMRD_Msk |
			FMC_SDTR1_TXSR_Msk |
			FMC_SDTR1_TRAS_Msk |
			FMC_SDTR1_TRC_Msk |
			FMC_SDTR1_TWR_Msk |
			FMC_SDTR1_TRP_Msk |
			FMC_SDTR1_TRCD_Msk;
	reg  = 	1 << FMC_SDTR1_TMRD_Pos |	// 1 cycle : uncertain !
			8 << FMC_SDTR1_TXSR_Pos |	// 8 x 9.26 = 74.08ns > tXSR(70ns)
			3 << FMC_SDTR1_TRAS_Pos |	// 3 x 9.26 = 27.78ns < tRFC(70ns) : uncertain !
			8 << FMC_SDTR1_TRC_Pos |	// 8 x 9.26 = 74.08ns > tRC(70ns)
			3 << FMC_SDTR1_TWR_Pos |	// 3 x 9.26 = 27.78ns > tWR(1CLK+7ns or 14ns) : uncertain !
			3 << FMC_SDTR1_TRP_Pos |	// 3 x 9.26 = 27.78ns > tRP(20ns)
			3 << FMC_SDTR1_TRCD_Pos;	// 3 x 9.26 = 27.78ns < tRCD(20ns)
	FMC_Bank5_6->SDTR[0] &= ~mask;
	FMC_Bank5_6->SDTR[0] |= reg;
	while ((FMC_Bank5_6->SDTR[0] & mask) != reg);

	// --- Set SDRAM-MODE register ---
	// 3. Set MODE bits to ‘001’ and configure the Target Bank bits
	//    (CTB1 and/or CTB2) in the	FMC_SDCMR register to start delivering
	//    the clock to the memory (SDCKE is driven high).
	mask =	FMC_SDCMR_MODE_Msk |
			FMC_SDCMR_CTB1_Msk |
			FMC_SDCMR_NRFS_Msk |
			FMC_SDCMR_MRD_Msk;
	reg  = 	0b001	<< FMC_SDCMR_MODE_Pos |	// clock assign enable
			0b1		<< FMC_SDCMR_CTB1_Pos |	// CTB1 enable
			0		<< FMC_SDCMR_NRFS_Pos |	// NRFS not use
			0		<< FMC_SDCMR_MRD_Pos;	// MRD not use
	FMC_Bank5_6->SDCMR &= ~mask;
	FMC_Bank5_6->SDCMR |= reg;

	// 4. Wait during the prescribed delay period. Typical delay is around
	//    100 us (refer to the SDRAM data-sheet for the required delay after power-up).
	for (volatile uint32_t i = 0; i < 50000; i++); // at least 100 us delay here, (216MHz = 4.63ns, 100us / 2.63ns = 21598)

	// 5. Set MODE bits to ‘010’ and configure the Target Bank bits
	//    (CTB1 and/or CTB2) in the FMC_SDCMR register to issue a “Pre-charge All” command.
	reg  = 	0b010	<< FMC_SDCMR_MODE_Pos |	// pre-charge all
			0b1		<< FMC_SDCMR_CTB1_Pos |	// CTB1 enable
			0		<< FMC_SDCMR_NRFS_Pos |	// NRFS not use
			0		<< FMC_SDCMR_MRD_Pos;	// MRD not use
	FMC_Bank5_6->SDCMR &= ~mask;
	FMC_Bank5_6->SDCMR |= reg;

	// 6. Set MODE bits to ‘011’, and configure the Target Bank bits
	//    (CTB1 and/or CTB2) as well as the number of consecutive Auto-refresh
	//    commands (NRFS) in the FMC_SDCMR register.
	//    Refer to the SDRAM data-sheet for the number of Auto-refresh commands
	//    that should be issued. Typical number is 8.
	reg  = 	0b011	<< FMC_SDCMR_MODE_Pos |	// clock assign enable
			0b1		<< FMC_SDCMR_CTB1_Pos |	// CTB1 enable
			8		<< FMC_SDCMR_NRFS_Pos |	// Typical number is 8, but according to following MT48LC4M32B manual it's at least 2 and must delay 100us between twice auto-refresh command.
			0		<< FMC_SDCMR_MRD_Pos;	// MRD not use
	FMC_Bank5_6->SDCMR &= ~mask;
	FMC_Bank5_6->SDCMR |= reg;

	// 7. Configure the MRD field according to the SDRAM device, set the MODE bits
	//    to '100', and configure the Target Bank bits (CTB1 and/or CTB2) in
	//    the FMC_SDCMR register to issue a "Load Mode Register" command in order to
	//    program the SDRAM device. In particular:
	//    a) the CAS latency must be selected following configured value in
	//       FMC_SDCR1/2 registers
	//    b) the Burst Length (BL) of 1 must be selected by configuring the
	//       M[2:0] bits to 000 in the mode register. Refer to SDRAM device data-sheet.
	//    If the Mode Register is not the same for both SDRAM banks,
	//    this step has to be repeated twice, once for each bank,
	//    and the Target Bank bits set accordingly.

	// The mode register of MT48LC4M32B can refer to following definition.
	//  -|---------------|----|---------|--------------|----|--------------|-
	//   | 12 | 11 | 10  |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
	//  -|---------------|----|---------|--------------|----|--------------|-
	//   |  Reserved     | WB | Op Mode | CAS Latency  | BT | Burst Length |
	//  -|---------------|----|---------|--------------|----|--------------|-
	uint16_t MODEREG =	0b000	<< 0 |		// length = 1
						0b1		<< 3 |		// interleaved
						0b011	<< 4 |		// latency = 3
						0b00	<< 7 |		// standard
						0b1		<< 9;		// single
	reg  = 	0b100	<< FMC_SDCMR_MODE_Pos |	// load mode register
			0b1		<< FMC_SDCMR_CTB1_Pos |	// CTB1 enable
			0		<< FMC_SDCMR_NRFS_Pos |	// not use
			MODEREG	<< FMC_SDCMR_MRD_Pos;	// Mode register
	FMC_Bank5_6->SDCMR &= ~mask;
	FMC_Bank5_6->SDCMR |= reg;

	// --- Set MCU-FMC register ---
	// 8. Program the refresh rate in the FMC_SDRTR register
	//    The refresh rate corresponds to the delay between refresh cycles.
	//    Its value must be adapted to SDRAM devices.
	FMC_Bank5_6->SDRTR &= ~FMC_SDRTR_COUNT_Msk;
	FMC_Bank5_6->SDRTR |= (uint32_t)1600 << FMC_SDRTR_COUNT_Pos;

	// The above parameter can be optimized according to the recommendation in SDRAM technical manual below.
	/*
	 * The following description is copied from MT48LC4M32B data-sheet.
	 * The recommended power-up sequence for SDRAM:
	 * 1.  Simultaneously apply power to VDD and VDDQ.
	 * 2.  Assert and hold CKE at a LVTTL logic LOW since all inputs and outputs are
	 *     LVTTL compatible.
	 * 3.  Provide stable CLOCK signal. Stable clock is defined as a signal cycling
	 *     within timing constraints specified for the clock pin.
	 * 4.  Wait at least 100˩s prior to issuing any command other than a
	 *     COMMAND INHIBIT or NOP.
	 * 5.  Starting at some point during this 100us period, bring CKE HIGH.
	 *     Continuing at least through the end of this period, 1 or more
	 *     COMMAND INHIBIT or NOP commands must be applied.
	 * 6.  Perform a PRECHARGE ALL command.
	 * 7.  Wait at least tRP time; during this time NOPs or DESELECT commands must
	 *     be given. All banks will complete their pre-charge, thereby placing
	 *     the device in the all banks idle state.
	 * 8.  Issue an AUTO REFRESH command.
	 * 9.  Wait at least tRFC time, during which only NOPs or COMMAND INHIBIT
	 *     commands are allowed.
	 * 10. Issue an AUTO REFRESH command.
	 * 11. Wait at least tRFC time, during which only NOPs or COMMAND INHIBIT
	 *     commands are allowed.
	 * 12. The SDRAM is now ready for mode register programming. Because the mode
	 *     register will power up in an unknown state, it should be loaded with
	 *     desired bit values prior to applying any operational command.
	 *     Using the LMR command, program the mode register.
	 *     The mode register is programmed via the MODE REGISTER SET command
	 *     with BA1 = 0, BA0 = 0 and retains the stored information until it is
	 *     programmed again or the device loses power.
	 *     Not programming the mode register upon initialization will result in
	 *     default settings which may not be desired. Outputs are guaranteed High-Z
	 *     after the LMR command is issued. Outputs should be High-Z already
	 *     before the LMR command is issued.
	 * 13. Wait at least tMRD time, during which only NOP or DESELECT commands
	 *     are allowed.
	 * At this point the DRAM is ready for any valid command.
	 *
	 */
}

// System tick initialization
static void initSystick(void)
{
	// Set RELOAD value (216000000 / 1000 = 216000)
	SysTick->LOAD = 0x00034BC0 - 1;

	// Set current value by default value
	//SysTick->VAL = 0x00000000;

	// Set CLKSOURCE, ENABLE
	SysTick->CTRL |= 	0b1 << SysTick_CTRL_CLKSOURCE_Pos |	// clk-source : processor clock
						0b1 << SysTick_CTRL_ENABLE_Pos;		// enable systick counter
}

// USART1 initialization
static void initUSART1(void)
{
	// Initialize USART1 TX/RX pin
	initUartGPIO();

	// Enable APB2 USART1 RCC
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	while ((RCC->APB2ENR & RCC_APB2ENR_USART1EN_Msk) == 0);

	// Set USART1 parameter
	//USART1->BRR = 0x2BF2;	// baud rate = 9600(fCK=108MHz, CR1/OVER8=0, 0x2BF2 = 108000000/9600)
	USART1->BRR = 0x03AA;	// baud rate = 15200(fCK=108MHz, CR1/OVER8=0, 0x03AA = 108000000/115200)
	USART1->CR1 &= ~USART_CR1_M;	// data bits = 8(M[1:0]=00)
	USART1->CR2 &= ~USART_CR2_STOP;	// stop bits = 1(STOP[1:0]=00)
	USART1->CR1 &= ~USART_CR1_PCE;	// parity = off(parity = on, odd:0x00000600, even:0x00000400)
	USART1->CR3 &= ~(USART_CR3_CTSE | USART_CR3_RTSE); // disable CTS and RTS

	// Disable RX interrupt in console log mode
	//USART1->CR1 |&= USART_CR1_RXNEIE;

	// Enable USART1
	USART1->CR1 |= USART_CR1_UE;

	// Enable TX and RE
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;
}

// Touch panel initialization (I2C3 and EXIT13)
static void initTouchPanel(void)
{
	// ---- Initialize I2C3 ----
	// Initialize GPIO for I2C3
	initTouchPanelGPIO();

	// Enable I2C3 RCC
	RCC->APB1ENR	|=	RCC_APB1ENR_I2C3EN;
	while ((RCC->APB1ENR & RCC_APB1ENR_I2C3EN_Msk) == 0);

	// Set I2C3 clock
	RCC->DCKCFGR2	&=	~RCC_DCKCFGR2_I2C3SEL_Msk;
	RCC->DCKCFGR2	|=	0b00 << RCC_DCKCFGR2_I2C3SEL_Pos; // APB1 clock (54MHz)

	// Set I2C3 register
	I2C3->CR1		&=	~I2C_CR1_PE_Msk;
	I2C3->TIMINGR	=	0x30C03444; // calculate by CubeMX (I2C Frequency = 100KHz, Rise time = 700ns, Fall time = 100ns)
	//I2C3->TIMINGR	=	0x40912732; // from ST sample
	//I2C3->TIMINGR	=	0x20404768; // from https://github.com/haidongqing/i2c3test-xy/blob/master/
	I2C3->OAR1		&=	~(I2C_OAR1_OA1_Msk | I2C_OAR1_OA1MODE_Msk | I2C_OAR1_OA1EN_Msk);
	I2C3->OAR1		|=	0b1 << I2C_OAR1_OA1EN_Pos;

	// Reset I2C3
	while (I2C3->CR1 & I2C_CR1_PE_Msk);
	I2C3->CR1		|=	I2C_CR1_PE;

	// ---- Initialize EXTI13 ----
	// Mapping PI13 in SYSCFG register
	RCC->APB2ENR	|=	RCC_APB2ENR_SYSCFGEN;
	while ((RCC->APB2ENR & RCC_APB2ENR_SYSCFGEN) == 0);
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13_Msk;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PI;

	// Initialize EXTI15_10 interrupt
	EXTI->IMR		|=	EXTI_IMR_MR13;  // EXT INT 13
	EXTI->RTSR		|=	EXTI_RTSR_TR13; // Rising edge trigger

	// Enable FT5336 interrupt to host
	enableFT5336Int();
	//printf("GMODE = %d.\r\n", (int)i2c3Read1Byte(0x70, 0xA4));
}

// LTDC initialization
static void initLCD(void)
{
	// Initialize GPIO for LTDC
	initLCDGPIO();

	// Initialize DMA2D
	initDMA2D();

	// 1. Enable the LTDC clock in the RCC register.
	RCC->APB2ENR	|=	RCC_APB2ENR_LTDCEN;
	while ((RCC->APB2ENR & RCC_APB2ENR_LTDCEN_Msk) == 0);

	// 2. Configure the required pixel clock following the panel datasheet.
	RCC->DCKCFGR1	&=	~RCC_DCKCFGR1_PLLSAIDIVR_Msk;
	RCC->DCKCFGR1	|=	0b10 << RCC_DCKCFGR1_PLLSAIDIVR_Pos; // PLLSAIDIVR = 8
	RCC->PLLSAICFGR = 	384 << RCC_PLLSAICFGR_PLLSAIN_Pos |
						5 << RCC_PLLSAICFGR_PLLSAIR_Pos |
						2 << RCC_PLLSAICFGR_PLLSAIQ_Pos | // not used
						0 << RCC_PLLSAICFGR_PLLSAIP_Pos;  // not used
	RCC->CR |= RCC_CR_PLLSAION;
	while ((RCC->CR & RCC_CR_PLLSAIRDY_Msk) == 0);

	//  The following graph is copied from ST sample code.
	//  LCD_TFT synchronous timings configuration :
	//  -------------------------------------------
	//
	//                                             Total Width
	//                          <--------------------------------------------------->
	//                    Hsync width HBP             Active Width                HFP
	//                          <---><--><--------------------------------------><-->
	//                      ____    ____|_______________________________________|____
	//                          |___|   |                                       |    |
	//                                  |                                       |    |
	//                      __|         |                                       |    |
	//         /|\    /|\  |            |                                       |    |
	//          | VSYNC|   |            |                                       |    |
	//          |Width\|/  |__          |                                       |    |
	//          |     /|\     |         |                                       |    |
	//          |  VBP |      |         |                                       |    |
	//          |     \|/_____|_________|_______________________________________|    |
	//          |     /|\     |         | / / / / / / / / / / / / / / / / / / / |    |
	//          |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
	// Total    |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
	// Heigh    |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
	//          |Active|      |         |/ / / / / / / / / / / / / / / / / / / /|    |
	//          |Heigh |      |         |/ / / / / / Active Display Area / / / /|    |
	//          |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
	//          |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
	//          |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
	//          |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
	//          |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
	//          |     \|/_____|_________|_______________________________________|    |
	//          |     /|\     |                                                      |
	//          |  VFP |      |                                                      |
	//         \|/    \|/_____|______________________________________________________|
	//
	//
	//  Each LCD device has its specific synchronous timings values.
	//  This example uses AM480272H3TMQW-T01H LCD (MB1046 B-01) and configures
	//  the synchronous timings as following :
	//
	//  ********************************** !!!! Attention !!!! **********************************
	//  ST sample code is written for AM480272H3TMQW-T01H LCD (MB1046 B-01). It just can support RGB565
	//  (16 bits/pixel). But my board is MB1191 B-02, and it's LCD RK043FN48H-CT672B can support RGB888
	//  (24 bits/pixel). In order to improve the quality of LCD display, the layer register PFCR and CFBLR
	//  setting value should be modified.
	//  *****************************************************************************************
	//
	//  Horizontal Synchronization (Hsync) = 41
	//  Horizontal Back Porch (HBP)        = 13
	//  Active Width                       = 480
	//  Horizontal Front Porch (HFP)       = 32
	//
	//  Vertical Synchronization (Vsync)   = 10
	//  Vertical Back Porch (VBP)          = 2
	//  Active Height                      = 272
	//  Vertical Front Porch (VFP)         = 2
	//

	// 3. Configure the synchronous timings: VSYNC, HSYNC, vertical and horizontal back
	//    porch, active data area and the front porch timings following the panel datasheet as
	//    described in the Section 18.4.1: LTDC global configuration parameters.
	LTDC->SSCR	= 	(LCD_HSYNC - 1)											<< LTDC_SSCR_HSW_Pos |
					(LCD_VSYNC - 1)											<< LTDC_SSCR_VSH_Pos;
	LTDC->BPCR	=	(LCD_HSYNC + LCD_HBP - 1)								<< LTDC_BPCR_AHBP_Pos |
					(LCD_VSYNC + LCD_VBP - 1) 								<< LTDC_BPCR_AVBP_Pos;
	LTDC->AWCR	=	(LCD_HSYNC + LCD_HBP + LCD_ACTIVE_WIDTH - 1) 			<< LTDC_AWCR_AAW_Pos |
					(LCD_VSYNC + LCD_VBP + LCD_ACTIVE_HEIGHT - 1) 			<< LTDC_AWCR_AAH_Pos;
	LTDC->TWCR	=	(LCD_HSYNC + LCD_HBP + LCD_ACTIVE_WIDTH + LCD_HFP - 1) 	<< LTDC_TWCR_TOTALW_Pos |
					(LCD_VSYNC + LCD_VBP + LCD_ACTIVE_HEIGHT + LCD_VFP - 1) << LTDC_TWCR_TOTALH_Pos;
	// while ();

	// 4. Configure the synchronous signals and clock polarity in the LTDC_GCR register.
	// (*) Polarity assignment set by default value (low level valid).

	// 5. If needed, configure the background color in the LTDC_BCCR register.
	// (*) Background color set by default value (RGB888:0x000000).

	// 6. Configure the needed interrupts in the LTDC_IER and LTDC_LIPCR register.
	// (*) Temporarily donn't enable LTDC interrupt.

	// 7. Configure the layer1/2 parameters by:
	// ---- for layer1 ----
	//    – programming the layer window horizontal and vertical position in the
	//      LTDC_LxWHPCR and LTDC_WVPCR registers. The layer window must be in the
	//      active data area.
	uint16_t bpcrAhbp1	=	((LTDC->BPCR & LTDC_BPCR_AHBP_Msk) >> LTDC_BPCR_AHBP_Pos) + 1;
	uint16_t bpcrAvbp1	=	((LTDC->BPCR & LTDC_BPCR_AVBP_Msk) >> LTDC_BPCR_AVBP_Pos) + 1;
	LTDC_Layer1->WHPCR	=	(0 + bpcrAhbp1) << LTDC_LxWHPCR_WHSTPOS_Pos |
							(LCD_ACTIVE_WIDTH + bpcrAhbp1 - 1)   << LTDC_LxWHPCR_WHSPPOS_Pos;
	LTDC_Layer1->WVPCR	=	(0 + bpcrAvbp1 + 1) << LTDC_LxWVPCR_WVSTPOS_Pos |
							(LCD_ACTIVE_HEIGHT + bpcrAvbp1)   << LTDC_LxWVPCR_WVSPPOS_Pos;

	//    – programming the pixel input format in the LTDC_LxPFCR register
	LTDC_Layer1->PFCR	=	0b000; // ARGB8888

	//    – programming the color frame buffer start address in the LTDC_LxCFBAR register
	LTDC_Layer1->CFBAR	=	(uint32_t)&frameBuffer1; // Frame Buffer

	//    – programming the line length and pitch of the color frame buffer in the
	//      LTDC_LxCFBLR register
	LTDC_Layer1->CFBLR	=	(LCD_ACTIVE_WIDTH * LCD_FRAME_BUF_BYTES) << LTDC_LxCFBLR_CFBP_Pos |
							(LCD_ACTIVE_WIDTH * LCD_FRAME_BUF_BYTES + 3) << LTDC_LxCFBLR_CFBLL_Pos;

	//    – programming the number of lines of the color frame buffer in the
	//      LTDC_LxCFBLNR register
	LTDC_Layer1->CFBLNR	=	LCD_ACTIVE_HEIGHT;

	//    – if needed, loading the CLUT with the RGB values and its address in the
	//      LTDC_LxCLUTWR register
	// (*) Temporarily needn't CLUT function.

	//    – If needed, configuring the default color and the blending factors respectively in the
	//      LTDC_LxDCCR and LTDC_LxBFCR registers
	// (*) Default color and the blending factors respectively set by default value
	// (*) (constant alpha = 255, pixel alpha = 0, blending factor 1/2 = constant alpha * pixel alpha)

	// ---- for layer2 ----
	//    – programming the layer window horizontal and vertical position in the
	//      LTDC_LxWHPCR and LTDC_WVPCR registers. The layer window must be in the
	//      active data area.
	uint16_t bpcrAhbp2	=	((LTDC->BPCR & LTDC_BPCR_AHBP_Msk) >> LTDC_BPCR_AHBP_Pos) + 1;
	uint16_t bpcrAvbp2	=	((LTDC->BPCR & LTDC_BPCR_AVBP_Msk) >> LTDC_BPCR_AVBP_Pos) + 1;
	LTDC_Layer2->WHPCR	=	(0 + bpcrAhbp2) << LTDC_LxWHPCR_WHSTPOS_Pos |
							(LCD_ACTIVE_WIDTH + bpcrAhbp2 - 1)   << LTDC_LxWHPCR_WHSPPOS_Pos;
	LTDC_Layer2->WVPCR	=	(0 + bpcrAvbp2 + 1) << LTDC_LxWVPCR_WVSTPOS_Pos |
							(LCD_ACTIVE_HEIGHT + bpcrAvbp2)   << LTDC_LxWVPCR_WVSPPOS_Pos;

	//    – programming the pixel input format in the LTDC_LxPFCR register
	LTDC_Layer2->PFCR	=	0b000; // ARGB8888

	//    – programming the color frame buffer start address in the LTDC_LxCFBAR register
	LTDC_Layer2->CFBAR	=	(uint32_t)&frameBuffer2; // Frame Buffer

	//    – programming the line length and pitch of the color frame buffer in the
	//      LTDC_LxCFBLR register
	LTDC_Layer2->CFBLR	=	(LCD_ACTIVE_WIDTH * LCD_FRAME_BUF_BYTES) << LTDC_LxCFBLR_CFBP_Pos |
							(LCD_ACTIVE_WIDTH * LCD_FRAME_BUF_BYTES + 3) << LTDC_LxCFBLR_CFBLL_Pos;

	//    – programming the number of lines of the color frame buffer in the
	//      LTDC_LxCFBLNR register
	LTDC_Layer2->CFBLNR	=	LCD_ACTIVE_HEIGHT;

	//    – if needed, loading the CLUT with the RGB values and its address in the
	//      LTDC_LxCLUTWR register
	// (*) Temporarily needn't CLUT function.

	//    – If needed, configuring the default color and the blending factors respectively in the
	//      LTDC_LxDCCR and LTDC_LxBFCR registers
	// (*) Default color and the blending factors respectively set by default value
	// (*) (constant alpha = 255, pixel alpha = 0, blending factor 1/2 = constant alpha * pixel alpha)

	// 8. Enable layer1/2 and if needed the CLUT in the LTDC_LxCR register.
	LTDC_Layer1->CR |= LTDC_LxCR_LEN;
	LTDC_Layer2->CR |= LTDC_LxCR_LEN;

	// 9. If needed, enable dithering and color keying respectively in the LTDC_GCR and
	//    LTDC_LxCKCR registers. They can be also enabled on the fly.
	// (*) It need not dithering and color keying respectively function.

	// 10. Reload the shadow registers to active register through the LTDC_SRCR register.
	LTDC->SRCR |= LTDC_SRCR_IMR;

	// 11. Enable the LCD-TFT controller in the LTDC_GCR register.
	LTDC->GCR |= LTDC_GCR_LTDCEN;

	// 12. All layer parameters can be modified on the fly except the CLUT. The new configuration
	//    has to be either reloaded immediately or during vertical blanking period by configuring
	//    the LTDC_SRCR register.
}

static void initDMA2D(void)
{
	RCC->AHB1ENR	|= RCC_AHB1ENR_DMA2DEN;
	while ((RCC->AHB1ENR & RCC_AHB1ENR_DMA2DEN_Msk) == 0);
}

static void initLED1(void)
{
	initLED1GPIO();
}

static bool_t initSDCard(void)
{
	uint32_t resp  = 0;
	uint32_t volt  = 0;
	uint32_t addr  = 0;
	uint32_t count = 0xFFFF;

	if (!isSDCardInsert()) return False;

	// Initialize SDMMC interface to initialize mode (400KHz, 1bit)
	SDMMC1->CLKCR	=	0x76 << SDMMC_CLKCR_CLKDIV_Pos |	// 0x76+2=120, 48MHz/120=400KHz
						0b0 << SDMMC_CLKCR_HWFC_EN_Pos |	// disable hardware flow control
						0b00 << SDMMC_CLKCR_WIDBUS_Pos |	// 1 bit mode SDMMC_D0
						0b0  << SDMMC_CLKCR_PWRSAV_Pos |	// enable SDMMC_CK in SD bus inactive
						0b0  << SDMMC_CLKCR_BYPASS_Pos |	// disable bypass CLKDIV
						0b0 << SDMMC_CLKCR_NEGEDGE_Pos |	// rising edge R/W
						0b0 << SDMMC_CLKCR_CLKEN_Pos;		// disable SDMMC clock

	// Power on
	SDMMC1->POWER	=	SDMMC_POWER_PWRCTRL;				// power on
	for (volatile uint32_t i = 0; i < 1000000; i++);		// delay > 1ms
	SDMMC1->CLKCR	|=	0b1 << SDMMC_CLKCR_CLKEN_Pos;		// enable SDMMC clock

	// Send CMD0 to identify card operating voltage.
	if (!sdmmcSendCmd(SD_CMD_GO_IDLE_STATE, SD_RESPONSE_R0, 0)) return False;

	// Send CMD8 to verify SD card version.
	// [11:8]: Supply Voltage (VHS) 0x1 (Range: 2.7-3.6 V)
	// [7:0]:  Check Pattern (recommended 0xAA) */
	if (sdmmcSendCmd(SD_CMD_HS_SEND_EXT_CSD, SD_RESPONSE_R7, 0x1AA)) {
		sdcard.version = SD_VERSION_2X;
	}
	else {
		sdcard.version = SD_VERSION_1X;
	}

	// Repeat sending CMD41 until SD card is ready.
	do {
		// Send CMD55 to change command mode to application command.
		if (!sdmmcSendCmd(SD_CMD_APP_CMD, SD_RESPONSE_R1, 0)) return False;

		// Send ACMD41 to judge if SD card is ready.
		// SDMMC_VOLTAGE_WINDOW_SD	: 0x80100000
		// SDMMC_HIGH_CAPACITY		: 0x40000000
		// SD_SWITCH_1_8V_CAPACITY	: 0x01000000
		if (!sdmmcSendCmd(SD_CMD_APP_OP_COND, SD_RESPONSE_R3, 0xC1100000)) return False;

		// Get operating voltage
		resp = SDMMC1->RESP1;
		volt = (((resp >> 31) == 1) ? 1 : 0);
		count--;
	}
	while (volt == 0 && count != 0);
	if (count == 0) return False;
	if (resp & 0x40000000) {
		sdcard.type = SD_TYPE_SDHC_SDXC;
	}
	else {
		sdcard.type = SD_TYPE_SDSC;
	}

	// Send CMD2 to get CID.
	if (!sdmmcSendCmd(SD_CMD_ALL_SEND_CID, SD_RESPONSE_R2, 0)) return False;

	// Send CMD3 to assign SD relative card address.
	if (!sdmmcSendCmd(SD_CMD_SET_REL_ADDR, SD_RESPONSE_R6, 0)) return False;
	addr = sdcard.rca << 16;

	// Send CMD9 to receive CSD information.
	if (!sdmmcSendCmd(SD_CMD_SEND_CSD, SD_RESPONSE_R2, 0)) return False;

	// Send CMD7 to select the current card.
	if (!sdmmcSendCmd(SD_CMD_SEL_DESEL_CARD, SD_RESPONSE_R1, addr)) return False;

	// --- SD card enumeration process is completed ! ---

	// Send CMD16 to set card block size to 8 bytes.
	if (!sdmmcSendCmd(SD_CMD_SET_BLOCKLEN, SD_RESPONSE_R1, 8)) return False;

	// Set SDMMC interface register to configure data length to 8 bytes
	SDMMC1->DTIMER	=	0xFFFFFFFF;
	SDMMC1->DLEN	=	8; // 8 Bytes
	SDMMC1->DCTRL	=	0b0011 << SDMMC_DCTRL_DBLOCKSIZE_Pos |	// 8 Bytes
						0b1 << SDMMC_DCTRL_DTDIR_Pos |			// card --> SDMMC
						0b0 << SDMMC_DCTRL_DTMODE_Pos |			// block mode
						0b1 << SDMMC_DCTRL_DTEN_Pos;			// enable data transfer

	// Send CMD55 to change command mode to application command.
	if (!sdmmcSendCmd(SD_CMD_APP_CMD, SD_RESPONSE_R1, addr)) return False;

	// Send ACMD51 to get bus wide information from SD card SCR register.
	if (!sdmmcSendCmd(SD_CMD_APP_SEND_SCR, SD_RESPONSE_R1, 0)) return False;
	// Read all SCR register from data FIFO
	uint32_t scr[2] = {0, 0};
	uint32_t* pSCR = (uint32_t*)&scr[0];
	uint32_t errMask = SDMMC_STA_RXOVERR_Msk | SDMMC_STA_DCRCFAIL_Msk | SDMMC_STA_DTIMEOUT_Msk;
	while ((SDMMC1->STA & errMask) == 0) {
		if (SDMMC1->STA & SDMMC_STA_RXDAVL_Msk) *pSCR++ = SDMMC1->FIFO;
		else if (!(SDMMC1->STA & SDMMC_STA_RXACT_Msk)) break;
	}
	if (SDMMC1->STA & errMask) {
		SDMMC1->ICR |= 	SDMMC1->STA & errMask;
		return False;
	}
	// Change SCR data from big endian to little endian.
	sdcard.scr.raw[0] =	(scr[1] & 0x000000FF) << 24 |
						(scr[1] & 0x0000FF00) << 8  |
						(scr[1] & 0x00FF0000) >> 8  |
						(scr[1] & 0xFF000000) >> 24;
	sdcard.scr.raw[1] =	(scr[0] & 0x000000FF) << 24 |
						(scr[0] & 0x0000FF00) << 8  |
						(scr[0] & 0x00FF0000) >> 8  |
						(scr[0] & 0xFF000000) >> 24;
	sdcard.scr.wideBus = (sdcard.scr.raw[1] & 0x00040000) != 0;

	if (sdcard.scr.wideBus) {
		// Send CMD55 to change command mode to application command.
		if (!sdmmcSendCmd(SD_CMD_APP_CMD, SD_RESPONSE_R1, addr)) return False;

		// Send ACMD6 to set wide bus mode.
		if (!sdmmcSendCmd(SD_CMD_APP_SET_BUSWIDTH, SD_RESPONSE_R1, 2)) return False;
	}

	// Initialize SDMMC interface to transfer mode (24MHz, 4bits)
	SDMMC1->CLKCR	|=	0x00 << SDMMC_CLKCR_CLKDIV_Pos |	// 0+2=2, 48MHz/2=24MHz
						0b1 << SDMMC_CLKCR_HWFC_EN_Pos |	// disable hardware flow control
						0b01 << SDMMC_CLKCR_WIDBUS_Pos |	// 4 bits mode SDMMC_D[0:3]
						0b0  << SDMMC_CLKCR_PWRSAV_Pos |	// enable SDMMC_CK in SD bus inactive (disable power save mode)
						0b0  << SDMMC_CLKCR_BYPASS_Pos |	// disable bypass CLKDIV
						0b0 << SDMMC_CLKCR_NEGEDGE_Pos;		// rising edge R/W
						//0b1 << SDMMC_CLKCR_CLKEN_Pos;		// enable SDMMC clock

	// Send CMD16 to set card block size.
	if (!sdmmcSendCmd(SD_CMD_SET_BLOCKLEN, SD_RESPONSE_R1, SD_BLOCKSIZE)) return False;

	// --- SD card data transfer preparation is completed ! ---

	return True;
}

static void initSDMMC(void)
{
	// Set GPIO
	initSDMMCGPIO();

	// Enable RRC
	RCC->APB2ENR	|=	RCC_APB2ENR_SDMMC1EN;
	while ((RCC->APB2ENR & RCC_APB2ENR_SDMMC1EN_Msk) == 0);

	RCC->AHB1ENR	|=	RCC_AHB1ENR_DMA2EN;
	while ((RCC->AHB1ENR & RCC_AHB1ENR_DMA2EN_Msk) == 0);

	// Set SDMMC clock
	RCC->DCKCFGR2	&=	~RCC_DCKCFGR2_CK48MSEL_Msk;
	RCC->DCKCFGR2	|=	0b0 << RCC_DCKCFGR2_CK48MSEL_Pos;	// 48MHz from PLLQ
	RCC->DCKCFGR2	&=	~RCC_DCKCFGR2_SDMMC1SEL_Msk;
	RCC->DCKCFGR2	|=	0b0 << RCC_DCKCFGR2_SDMMC1SEL_Pos;	// select 48MHz for SDMMC

	// ----- SDMMC RX DMA setting -----
	// Configure SDMMC RX DMA
	DMA2_Stream3->CR	&=	~DMA_SxCR_EN_Msk;
	DMA2_Stream3->CR	= 	0b100	<< DMA_SxCR_CHSEL_Pos |		// stream3, channel4
							0b00	<< DMA_SxCR_DIR_Pos |		// peripheral to memory
							0b0		<< DMA_SxCR_PINC_Pos |		// peripheral increment disable
							0b1		<< DMA_SxCR_MINC_Pos |		// memory increment enable
							0b10	<< DMA_SxCR_PSIZE_Pos |		// peripheral data size = word
							0b10	<< DMA_SxCR_MSIZE_Pos |		// memory data size = word
							0b1		<< DMA_SxCR_PFCTRL_Pos |	// PFC (peripheral is flow controller)
							0b11	<< DMA_SxCR_PL_Pos |		// highest DMA priority
							0b01	<< DMA_SxCR_PBURST_Pos |	// peripheral burst = 4 bytes
							0b01	<< DMA_SxCR_MBURST_Pos;		// peripheral burst = 4 bytes
	DMA2_Stream3->FCR	=	0b1		<< DMA_SxFCR_DMDIS_Pos |	// disable direct mode
							0b11	<< DMA_SxFCR_FTH_Pos;		// FIFO full

	// Clear SDMMC RX DMA interrupt flag
	DMA2->LIFCR			|= 	DMA_LIFCR_CTCIF3_Msk |
							DMA_LIFCR_CTEIF3_Msk |
							DMA_LIFCR_CDMEIF3_Msk |
							DMA_LIFCR_CFEIF3_Msk;

	// Set SDMMC RX DMA interrupt mask
	DMA2_Stream3->CR	|=	DMA_SxCR_TCIE_Msk | DMA_SxCR_TEIE_Msk | DMA_SxCR_DMEIE_Msk;
	DMA2_Stream3->FCR	|=	DMA_SxFCR_FEIE_Msk;

	// Enable SDMMC RX DMA
	//DMA2_Stream3->CR	|=	DMA_SxCR_EN_Msk;

	// ----- SDMMC TX DMA setting -----
	// Configure SDMMC TX DMA
	DMA2_Stream6->CR	&=	~DMA_SxCR_EN_Msk;
	DMA2_Stream6->CR	=	0b100	<< DMA_SxCR_CHSEL_Pos |		// stream6, channel4
							0b01	<< DMA_SxCR_DIR_Pos |		// memory to peripheral
							0b0		<< DMA_SxCR_PINC_Pos |		// peripheral increment disable
							0b1		<< DMA_SxCR_MINC_Pos |		// memory increment enable
							0b10	<< DMA_SxCR_PSIZE_Pos |		// peripheral data size = word
							0b10	<< DMA_SxCR_MSIZE_Pos |		// memory data size = word
							0b1		<< DMA_SxCR_PFCTRL_Pos |	// PFC (peripheral is flow controller)
							0b11	<< DMA_SxCR_PL_Pos |		// highest DMA priority
							0b01	<< DMA_SxCR_PBURST_Pos |	// peripheral burst = 4 bytes
							0b01	<< DMA_SxCR_MBURST_Pos;		// peripheral burst = 4 bytes
	DMA2_Stream6->FCR	= 	0b1		<< DMA_SxFCR_DMDIS_Pos |	// disable direct mode
							0b11	<< DMA_SxFCR_FTH_Pos;		// FIFO full

	// Clear SDMMC TX DMA interrupt flag
	DMA2->HIFCR			|=	DMA_HIFCR_CTCIF6_Msk |
							DMA_HIFCR_CTEIF6_Msk |
							DMA_HIFCR_CDMEIF6_Msk |
							DMA_HIFCR_CFEIF6_Msk;

	// Set SDMMC TX DMA interrupt mask
	DMA2_Stream6->CR	|=	DMA_SxCR_TCIE_Msk | DMA_SxCR_TEIE_Msk | DMA_SxCR_DMEIE_Msk;
	DMA2_Stream6->FCR	|=	DMA_SxFCR_FEIE_Msk;

	// Enable SDMMC TX DMA
	//DMA2_Stream6->CR	|=	DMA_SxCR_EN_Msk;
}

#ifdef MODE_STAND_ALONE
static void initTIM7(void) // TIM7 initialization for low speed timer(500ms)
{
	// Enable APB1 TIM7 RCC
	RCC->APB1ENR	|= RCC_APB1ENR_TIM7EN;
	while ((RCC->APB1ENR & RCC_APB1ENR_TIM7EN_Msk) == 0);

	// Enable TIM7 update interrupt
	TIM7->DIER		|= TIM_DIER_UIE;

	// Initialize TIM7
	TIM7->CR1		|= TIM_CR1_ARPE;		// Enable pre-load ARPE
	TIM7->PSC		= 4000 - 1;				// timer frequency = 108MHz / 4000 = 27KHz (APB pre-sacle is not 1, so fCK_PSC = 54MHz * 2 = 108MHz)
	TIM7->CNT		&= TIM_CNT_UIFCPY_Msk;	// set bit[30:0] to 0
	TIM7->SR		&= ~TIM_SR_UIF;			// Clear timer update interrupt flag
	TIM7->ARR		= 13500 - 1;			// Set 13500 to reload value
	TIM7->CR1		|= TIM_CR1_CEN;			// Enable TIM7 CEN
}
#endif

// Set global NVIC priority
static void initNVICPriority(void)
{
	// STM32F756XX-PM can not refer here, because ST has own design in interrupt priority.
	// ST interrupt priority group setting can refer to the following.
	//   IPR just use the higher 4 bit indicating priority, the lower 4 bit should be set to 0.
	//   The assignment of AIRCR-PRIGROUP can be refer to the following rule.
	//   ----------------|------------------------------------------------
	//                   |                  IPRn / SHPRn
	//    AIRCR-PRIGROUP |------------------------------------------------
	//                   |  preemption priority  |    sub-priority
	//   ----------------|------------------------------------------------
	//     0xb011 :      |  [7:4], 16 level      |    None , None
	//     0xb100 :      |  [7:5], 8 level       |    [4]  , 2 level
	//     0xb011 :      |  [7:6], 4 level       |    [5:4], 4 level
	//     0xb011 :      |  [7]  , 2 level       |    [6:4], 8 level
	//     0xb011 :      |  None , None          |    [7:4], 16 level
	//   ----------------|------------------------------------------------

	// Set global NVIC priority group
	// "|=" can not be used here, because VECTKEY write key is "0x05FA" and "|=" will change it
	SCB->AIRCR =	0x05FA << SCB_AIRCR_VECTKEY_Pos |
					0b011  << SCB_AIRCR_PRIGROUP_Pos; // PRIGROUP = 0b011 (preemption:16, sub:0)
	while ((SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) != (0b011 << SCB_AIRCR_PRIGROUP_Pos));

	// Global interrupt design in sample project can refer to the following setting.
	// SVC(FreeRTOS)		: preemption:15, IRQn:11, This priority will be modified to lowest priority forcibly in RTOS.
	// PendSV(FreeRTOS)		: preemption:15, IRQn:14, This priority will be modified to lowest priority forcibly in RTOS.
	// Systick(FreeRTOS)	: preemption:15, IRQn:15, This priority will be modified to lowest priority forcibly in RTOS.
	// USART1(Debug)		: preemption:14, IRQn:37
	// TIMER7(LED1 flicker)	: preemption:14, IRQn:55
	// EXIT13(TouchPanel)	: preemption:13, IRQn:40
	// SDIO(FatFs)			: preemption:14, IRQn:49
	// SDDMARx(DMA2_Stream3): preemption:13, IRQn:59
	// SDDMATx(DMA2_Stream6): preemption:13, IRQn:69

	// Initialize SVCall interrupt (IRQn:11)
	SCB->SHPR[2] |= 15 << (24 + 4);
	// SVC need not open. Executing SVC instrument will trigger SVC interrupt.

	// Initialize PendSV interrupt (IRQn:14)
	SCB->SHPR[3] |= 15 << (16 + 4);
	// PendSV need not open. Setting ICSR the 28th bit will trigger pendSV interrupt.

	// Initialize Systick interrupt (IRQn:15)
	SCB->SHPR[3]  |= 15 << (24 + 4);
	SysTick->CTRL |= 0b1 << SysTick_CTRL_TICKINT_Pos;

	// Initialize USART1 interrupt (IRQn:37)
	NVIC->IP[37]  |= 14 << 4;
	NVIC->ISER[1] |= 0b1 << (37 - 32);

#ifdef MODE_STAND_ALONE
	// Initialize TIM7 interrupt (IRQn:55)
	NVIC->IP[13]  |= 14 << (24 + 4);
	NVIC->ISER[1] |= 0b1 << (55 - 32);
#endif

	// Initialize EXTI15_10 interrupt (IRQn:40)
	NVIC->IP[40]  |= 13 << 4;
	NVIC->ISER[1] |= 0b1 << (40 - 32);

	// Initialize SDMMC interrupt (IRQn:49)
	NVIC->IP[49]  |= 14 << 4;
	NVIC->ISER[1] |= 0b1 << (49 - 32);

	// Initialize SDIO DMARx (DMA2_Stream3) interrupt (IRQn:59)
	NVIC->IP[59]  |= 13 << 4;
	NVIC->ISER[1] |= 0b1 << (59 - 32);

	// Initialize SDIO DMATx (DMA2_Stream6) interrupt (IRQn:69)
	//NVIC->IP[69]  |= 13 << 4;
	//NVIC->ISER[2] |= 0b1 << (69 - 64);
}

#ifdef MODE_STAND_ALONE
static void initTIM7Int(void) // Initialize TIM7 interrupt
{
	// Set TIM7 interrupt priority for LED1 flicker
	NVIC->IP[13] |= 14 << (24 + 4);

	// Enable TIM7 global interrupt (IRQn=55)
	NVIC->ISER[1] |= 0b1 << (55 - 32);

	// Enable TIM7 update interrupt
	TIM7->DIER |= TIM_DIER_UIE;
}
#endif

// SDRAM pin initialization
//  FMC_SDCLK
//  FMC_NBL0   / FMC_NBL1
//  FMC_SDNRAS / FMC_SDNCAS
//  FMC_SDCKE0
//  FMC_SDNE0
//  FMC_SDNWE
//  FMC_BA[0:1]
//  FMC_A[0:11]
//  FMC_D[0:15]
static void initSDRAMGPIO(void)
{
	// GPIOC
	RCC->AHB1ENR	|=	RCC_AHB1ENR_GPIOCEN;
	while ((RCC->AHB1ENR & RCC_AHB1ENR_GPIOCEN_Msk) == 0);
	// PC3  --> FMC_SDCKE0
	GPIOC->MODER	|=	0b10	<< GPIO_MODER_MODER3_Pos;		// MODER = Multiple(0b10)
	GPIOC->OTYPER	|=	0b0		<< GPIO_OTYPER_OT3_Pos;			// OTYPER = PP
	GPIOC->OSPEEDR	|=	0b11	<< GPIO_OSPEEDR_OSPEEDR3_Pos;	// OSPEEDR = Full
	GPIOC->PUPDR	|=	0b00	<< GPIO_PUPDR_PUPDR3_Pos;		// PUPDR = No Pull
	GPIOC->AFR[0]	|=	12		<< GPIO_AFRL_AFRL3_Pos;			// AF12

	// GPIOD
	RCC->AHB1ENR	|=	RCC_AHB1ENR_GPIODEN;
	while ((RCC->AHB1ENR & RCC_AHB1ENR_GPIODEN_Msk) == 0);
	// PD0  --> FMC_D2
	// PD1  --> FMC_D3
	// PD8  --> FMC_D13
	// PD9  --> FMC_D14
	// PD10 --> FMC_D15
	// PD14 --> FMC_D0
	// PD15 --> FMC_D1
	GPIOD->MODER	|=	0xA02A000A;	// MODER = Multiple(0b10)
	GPIOD->OTYPER	|=	0x00000000;	// OTYPER = PP
	GPIOD->OSPEEDR	|=	0xF03F000F;	// OSPEEDR = Full
	GPIOD->PUPDR	|=	0x00000000;	// PUPDR = No Pull
	GPIOD->AFR[0]	|=	0x000000CC;	// AF12
	GPIOD->AFR[1]	|=	0xCC000CCC;	// AF12

	// GPIOE
	RCC->AHB1ENR	|=	RCC_AHB1ENR_GPIOEEN;
	while ((RCC->AHB1ENR & RCC_AHB1ENR_GPIOEEN_Msk) == 0);
	// PE0  --> FMC_NBL0
	// PE1  --> FMC_NBL1
	// PE7  --> FMC_D4
	// PE8  --> FMC_D5
	// PE9  --> FMC_D6
	// PE10 --> FMC_D7
	// PE11 --> FMC_D8
	// PE12 --> FMC_D9
	// PE13 --> FMC_D10
	// PE14 --> FMC_D11
	// PE15 --> FMC_D12
	GPIOE->MODER	|=	0xAAAA800A;	// MODER = Multiple(0b10)
	GPIOE->OTYPER	|=	0x00000000;	// OTYPER = PP
	GPIOE->OSPEEDR	|=	0xFFFFC00F;	// OSPEEDR = Full
	GPIOE->PUPDR	|=	0x00000000;	// PUPDR = No Pull
	GPIOE->AFR[0]	|=	0xC00000CC;	// AF12
	GPIOE->AFR[1]	|=	0xCCCCCCCC;	// AF12

	// GPIOF
	RCC->AHB1ENR	|=	RCC_AHB1ENR_GPIOFEN;
	while ((RCC->AHB1ENR & RCC_AHB1ENR_GPIOFEN_Msk) == 0);
	// PF0  --> FMC_A0
	// PF1  --> FMC_A1
	// PF2  --> FMC_A2
	// PF3  --> FMC_A3
	// PF4  --> FMC_A4
	// PF5  --> FMC_A5
	// PF11 --> FMC_SDNRAS
	// PF12 --> FMC_A6
	// PF13 --> FMC_A7
	// PF14 --> FMC_A8
	// PF15 --> FMC_A9
	GPIOF->MODER	|=	0xAA800AAA;	// MODER = Multiple(0b10)
	GPIOF->OTYPER	|=	0x00000000;	// OTYPER = PP
	GPIOF->OSPEEDR	|=	0xFFC00FFF;	// OSPEEDR = Full
	GPIOF->PUPDR	|=	0x00000000;	// PUPDR = No Pull
	GPIOF->AFR[0]	|=	0x00CCCCCC;	// AF12
	GPIOF->AFR[1]	|=	0xCCCCC000;	// AF12

	// GPIOG
	RCC->AHB1ENR	|=	RCC_AHB1ENR_GPIOGEN;
	while ((RCC->AHB1ENR & RCC_AHB1ENR_GPIOGEN_Msk) == 0);
	// PG0  --> FMC_A10
	// PG1  --> FMC_A11
	// PG4  --> FMC_BA0
	// PG5  --> FMC_BA1
	// PG8  --> FMC_SDCLK
	// PG15 --> FMC_SDNCAS
	GPIOG->MODER	|=	0x80020A0A;	// MODER = Multiple(0b10)
	GPIOG->OTYPER	|=	0x00000000;	// OTYPER = PP
	GPIOG->OSPEEDR	|=	0xC0030F0F;	// OSPEEDR = Full
	GPIOG->PUPDR	|=	0x00000000;	// PUPDR = No Pull
	GPIOG->AFR[0]	|=	0x00CC00CC;	// AF12
	GPIOG->AFR[1]	|=	0xC000000C;	// AF12

	// GPIOH
	RCC->AHB1ENR	|=	RCC_AHB1ENR_GPIOHEN;
	while ((RCC->AHB1ENR & RCC_AHB1ENR_GPIOHEN_Msk) == 0);
	// PH3  --> FMC_SDNE0
	// PH5  --> FMC_SDNWE
	GPIOH->MODER	|=	0x00000880;	// MODER = Multiple(0b10)
	GPIOH->OTYPER	|=	0x00000000;	// OTYPER = PP
	GPIOH->OSPEEDR	|=	0x00000CC0;	// OSPEEDR = Full
	GPIOH->PUPDR	|=	0x00000000;	// PUPDR = No Pull
	GPIOH->AFR[0]	|=	0x00C0C000;	// AF12
}

// USART TX pin initialization
static void initUartGPIO(void)
{
	// GPIOA
	RCC->AHB1ENR	|=	RCC_AHB1ENR_GPIOAEN;
	while ((RCC->AHB1ENR & RCC_AHB1ENR_GPIOAEN_Msk) == 0);
	// PA9  --> USART1 TX
	GPIOA->MODER	|=	0b10	<< GPIO_MODER_MODER9_Pos;		// MODER = Multiple(0b10)
	GPIOA->OTYPER	|=	0b0		<< GPIO_OTYPER_OT9_Pos;
	GPIOA->OSPEEDR	|=	0b11	<< GPIO_OSPEEDR_OSPEEDR9_Pos;
	GPIOA->PUPDR	|=	0b01	<< GPIO_PUPDR_PUPDR9_Pos;
	GPIOA->AFR[1]	|=	7		<< GPIO_AFRH_AFRH1_Pos;			// AF7

	// GPIOB
	RCC->AHB1ENR	|=	RCC_AHB1ENR_GPIOBEN;
	while ((RCC->AHB1ENR & RCC_AHB1ENR_GPIOBEN_Msk) == 0);
	// PB7  --> USART1 RX
	GPIOB->MODER	|=	0b10	<< GPIO_MODER_MODER7_Pos;		// MODER = Multiple(0b10)
	GPIOB->OTYPER	|=	0b0		<< GPIO_OTYPER_OT9_Pos;
	// RX line need not to set OSPEEDR and PUPDR
	GPIOB->AFR[0]	|=	7		<< GPIO_AFRL_AFRL7_Pos;			// AF7
}

// LED1 pin initialization
static void initLED1GPIO(void)
{
	// GPIOI
	RCC->AHB1ENR	|=	RCC_AHB1ENR_GPIOIEN;
	while ((RCC->AHB1ENR & RCC_AHB1ENR_GPIOIEN_Msk) == 0);
	// PI1  --> LED1
	GPIOI->MODER	|=	0b01	<< GPIO_MODER_MODER1_Pos;		// MODER = Output
	GPIOI->OTYPER	|=	0b0		<< GPIO_OTYPER_OT1_Pos;
	GPIOI->OSPEEDR	|=	0b11	<< GPIO_OSPEEDR_OSPEEDR1_Pos;
	GPIOI->PUPDR	|=	0b01	<< GPIO_PUPDR_PUPDR1_Pos;
}

// LCD pin initialization
static void initLCDGPIO(void)
{
	// GPIOE
	RCC->AHB1ENR	|= RCC_AHB1ENR_GPIOEEN;
	while ((RCC->AHB1ENR & RCC_AHB1ENR_GPIOEEN_Msk) == 0);
	// PE4  --> LTDC_B0
	GPIOE->MODER	|=	0b10	<< GPIO_MODER_MODER4_Pos;		// MODER = Multiple(0b10)
	GPIOE->AFR[0]	|=	14		<< GPIO_AFRL_AFRL4_Pos;			// AF14

	// GPIOG
	RCC->AHB1ENR	|=	RCC_AHB1ENR_GPIOGEN;
	while ((RCC->AHB1ENR & RCC_AHB1ENR_GPIOGEN_Msk) == 0);
	// PG12 --> LTDC_B4
	GPIOG->MODER	|=	0b10	<< GPIO_MODER_MODER12_Pos;		// MODER = Multiple(0b10)
	GPIOG->AFR[1]	|=	14		<< GPIO_AFRH_AFRH4_Pos;			// AF14

	// GPIOI
	RCC->AHB1ENR	|=	RCC_AHB1ENR_GPIOIEN;
	while ((RCC->AHB1ENR & RCC_AHB1ENR_GPIOIEN_Msk) == 0);
	// PI9  --> LTDC_VSYNC
	// PI10 --> LTDC_HSYNC
	// PI14 --> LTDC_CLK
	// PI15 --> LTDC_R0
	GPIOI->MODER	|=	0b10	<< GPIO_MODER_MODER9_Pos |		// MODER = Multiple(0b10)
						0b10	<< GPIO_MODER_MODER10_Pos |
						0b10	<< GPIO_MODER_MODER14_Pos |
						0b10	<< GPIO_MODER_MODER15_Pos;
	GPIOI->AFR[1]	|=	14		<< GPIO_AFRH_AFRH1_Pos |		// AF14
						14		<< GPIO_AFRH_AFRH2_Pos |
						14		<< GPIO_AFRH_AFRH6_Pos |
						14		<< GPIO_AFRH_AFRH7_Pos;

	// GPIOJ
	RCC->AHB1ENR	|=	RCC_AHB1ENR_GPIOJEN;
	while ((RCC->AHB1ENR & RCC_AHB1ENR_GPIOJEN_Msk) == 0);
	// PJ0  --> LTDC_R1
	// PJ1  --> LTDC_R2
	// PJ2  --> LTDC_R3
	// PJ3  --> LTDC_R4
	// PJ4  --> LTDC_R5
	// PJ5  --> LTDC_R6
	// PJ6  --> LTDC_R7
	// PJ7  --> LTDC_G0
	// PJ8  --> LTDC_G1
	// PJ9  --> LTDC_G2
	// PJ10 --> LTDC_G3
	// PJ11 --> LTDC_G4
	// PJ13 --> LTDC_B1
	// PJ14 --> LTDC_B2
	// PJ15 --> LTDC_B3
	GPIOJ->MODER	|=	0xA8AAAAAA;	// MODER = Multiple(0b10)
	GPIOJ->AFR[0]	|=	0xEEEEEEEE; // AF14
	GPIOJ->AFR[1]	|=	0xEEE0EEEE; // AF14

	// GPIOK
	RCC->AHB1ENR	|=	RCC_AHB1ENR_GPIOKEN;
	while ((RCC->AHB1ENR & RCC_AHB1ENR_GPIOKEN_Msk) == 0);
	// PK0  --> LTDC_G5
	// PK1  --> LTDC_G6
	// PK2  --> LTDC_G7
	// PK4  --> LTDC_B5
	// PK5  --> LTDC_B6
	// PK6  --> LTDC_B7
	// PK7  --> LTDC_DE
	GPIOK->MODER	|=	0x0000AA2A;	// MODER = Multiple(0b10)
	GPIOK->AFR[0]	|=	0xEEEE0EEE; // AF = AF14

	// GPIOI12 --> LCD_DISP (must be manually controlled)
	GPIOI->MODER	|=	0b01 << GPIO_MODER_MODER12_Pos;		// MODER = Output(0b01)
	GPIOI->OTYPER	|=	0b0  << GPIO_OTYPER_OT12_Pos;		// OTYPER = PP
	GPIOI->BSRR		|=	0b1  << GPIO_BSRR_BS12_Pos;			// Bit Set

	// GPIOK3  --> LCD_BL_CTRL (must be manually controlled)
	GPIOK->MODER	|=	0b01 << GPIO_MODER_MODER3_Pos;		// MODER = Output(0b01)
	GPIOK->OTYPER	|=	0b0  << GPIO_OTYPER_OT3_Pos;		// OTYPER = PP
	GPIOK->BSRR		|=	0b1  << GPIO_BSRR_BS3_Pos;			// Bit Set
}

// Touch panel pin initialization
static void initTouchPanelGPIO(void)
{
	// GPIOH RCC
	RCC->AHB1ENR	|=	RCC_AHB1ENR_GPIOHEN;
	while ((RCC->AHB1ENR & RCC_AHB1ENR_GPIOHEN_Msk) == 0);
	// PH7  --> I2C3_SCL (for FT5336 Control)
	// PH8  --> I2C3_SDA (for FT5336 Control)
	GPIOH->MODER	|=	0b10	<< GPIO_MODER_MODER7_Pos |		// MODER = Multiple(0b10)
						0b10	<< GPIO_MODER_MODER8_Pos;
	GPIOH->OTYPER	|=	0b1		<< GPIO_OTYPER_OT7_Pos |		// Open Drain
						0b1		<< GPIO_OTYPER_OT8_Pos;
	GPIOH->OSPEEDR	|=	0b11	<< GPIO_OSPEEDR_OSPEEDR7_Pos |	// Very high speed
						0b11	<< GPIO_OSPEEDR_OSPEEDR8_Pos;
	GPIOH->PUPDR	|=	0b01	<< GPIO_PUPDR_PUPDR7_Pos |		// Pull-up
						0b01	<< GPIO_PUPDR_PUPDR8_Pos;
	GPIOH->AFR[0]	|=	4		<< GPIO_AFRL_AFRL7_Pos;			// AF4
	GPIOH->AFR[1]	|=	4		<< GPIO_AFRH_AFRH0_Pos;			// AF4

	// GPIOI RCC
	RCC->AHB1ENR	|=	RCC_AHB1ENR_GPIOIEN;
	while ((RCC->AHB1ENR & RCC_AHB1ENR_GPIOIEN_Msk) == 0);
	// PI13 --> LCD_INT (for FT5336 Control)
	GPIOI->MODER	|=	0b00	<< GPIO_MODER_MODER13_Pos;		// MODER = Multiple(0b10)
	GPIOI->OTYPER	|=	0b0		<< GPIO_OTYPER_OT13_Pos;		// Push and pull
	GPIOI->OSPEEDR	|=	0b10	<< GPIO_OSPEEDR_OSPEEDR13_Pos;	// High speed
	GPIOI->PUPDR	|=	0b00	<< GPIO_PUPDR_PUPDR13_Pos;		// No Pull
}

// SDMMC pin initialization
static void initSDMMCGPIO(void)
{
	// GPIOC RCC
	RCC->AHB1ENR	|=	RCC_AHB1ENR_GPIOCEN;
	while ((RCC->AHB1ENR & RCC_AHB1ENR_GPIOCEN_Msk) == 0);
	// PC8  --> SDMMC D0
	// PC9  --> SDMMC D1
	// PC10 --> SDMMC D2
	// PC11 --> SDMMC D3
	// PC12 --> SDMMC CK
	GPIOC->MODER	|=	0x02AA0000;	// MODER = Multiple(0b10)
	GPIOC->OTYPER	|=	0x00000000;	// Push and pull
	GPIOC->OSPEEDR	|=	0x02AA0000;	// High speed
	GPIOC->PUPDR	|=	0x01550000;	// Pull-up
	GPIOC->AFR[1]	|=	0x000CCCCC; // AF12

	// PC13 --> MicroSDcard detect
	GPIOC->MODER	|=	0b00	<< GPIO_MODER_MODER13_Pos;		// MODER = Input(0b00)
	GPIOC->OSPEEDR	|=	0b10	<< GPIO_OSPEEDR_OSPEEDR13_Pos;	// High speed
	GPIOC->PUPDR	|=	0b01	<< GPIO_PUPDR_PUPDR13_Pos;		// Pull-up

	// GPIOD RCC
	RCC->AHB1ENR	|=	RCC_AHB1ENR_GPIODEN;
	while ((RCC->AHB1ENR & RCC_AHB1ENR_GPIODEN_Msk) == 0);
	// PD2  --> SDMMC CMD
	GPIOD->MODER	|=	0b10	<< GPIO_MODER_MODER2_Pos;		// MODER = Multiple(0b10)
	GPIOD->OTYPER	|=	0b0		<< GPIO_OTYPER_OT2_Pos;			// Push and pull
	GPIOD->OSPEEDR	|=	0b10	<< GPIO_OSPEEDR_OSPEEDR2_Pos;	// High speed
	GPIOD->PUPDR	|=	0b01	<< GPIO_PUPDR_PUPDR2_Pos;		// Pull-up
	GPIOD->AFR[0]	|=	12		<< GPIO_AFRL_AFRL2_Pos;			// AF12
}

static void setCharBuf06x08(
		const uint16_t symbol,
		const uint32_t foreColor,
		const uint32_t backColor)
{
	assert_param(symbol < 128);

	uint32_t height = sFont.st.head.fontHeight;
	uint32_t width  = sFont.st.cdef[symbol].width;

	uint32_t cnt = 0;
	for (uint32_t i = 0; i < height; i++) {
		for (uint32_t j = 0; j < width; j++) {
			// Because font file is rotated 90 degrees clockwise, the following i and j are exchanged.
			if (sFont.st.cdef[symbol].data[j][0] & (0b1 << (7 - i))) { // use fore color
				charBuffer[cnt++] = (foreColor >>  0) & 0xFF; // blue
				charBuffer[cnt++] = (foreColor >>  8) & 0xFF; // green
				charBuffer[cnt++] = (foreColor >> 16) & 0xFF; // red
				charBuffer[cnt++] = (foreColor >> 24) & 0xFF; // alpha
			}
			else { // use back color
				charBuffer[cnt++] = (backColor >>  0) & 0xFF; // blue
				charBuffer[cnt++] = (backColor >>  8) & 0xFF; // green
				charBuffer[cnt++] = (backColor >> 16) & 0xFF; // red
				charBuffer[cnt++] = (backColor >> 24) & 0xFF; // alpha
			}
		}
	}
}

static void setCharBuf12x16(
		const uint16_t symbol,
		const uint32_t foreColor,
		const uint32_t backColor)
{
	assert_param(symbol < 128);

	uint32_t height = mFont.st.head.fontHeight;
	uint32_t width  = mFont.st.cdef[symbol].width;

	uint32_t cnt = 0;
	for (uint32_t i = 0; i < height; i++) {
		for (uint32_t j = 0; j < width; j++) {
			// Because font file is rotated 90 degrees clockwise, the following i and j are exchanged.
			if (mFont.st.cdef[symbol].data[j][i / 8] & (0b1 << (7 - i % 8))) { // use fore color
				charBuffer[cnt++] = (foreColor >>  0) & 0xFF; // blue
				charBuffer[cnt++] = (foreColor >>  8) & 0xFF; // green
				charBuffer[cnt++] = (foreColor >> 16) & 0xFF; // red
				charBuffer[cnt++] = (foreColor >> 24) & 0xFF; // alpha
			}
			else { // use back color
				charBuffer[cnt++] = (backColor >>  0) & 0xFF; // blue
				charBuffer[cnt++] = (backColor >>  8) & 0xFF; // green
				charBuffer[cnt++] = (backColor >> 16) & 0xFF; // red
				charBuffer[cnt++] = (backColor >> 24) & 0xFF; // alpha
			}
		}
	}
}

static bool_t sdmmcCheckCmdResp0(void)
{
	uint32_t count = SD_SEND_CMD_TIMEOUT_CNT;
	while ((SDMMC1->STA & SDMMC_STA_CMDSENT_Msk) == 0 && count-- != 0);
	SDMMC1->ICR |= 	SDMMC1->STA;

	return count != 0; // count == 0 : timeout
}

static bool_t sdmmcCheckCmdResp1(uint8_t cmd)
{
	uint32_t count = SD_SEND_CMD_TIMEOUT_CNT;
	if (cmd == SD_CMD_STOP_TRANSMISSION) count = 0xFFFFFFFF; //SD_STOP_TRANS_TIMEOUT_CNT;

	// Did not consider CCRCFAIL, CMDREND and CTIMEOUT error flags, because they will cause timeout.
	if (cmd != SD_CMD_STOP_TRANSMISSION) {
		while ((SDMMC1->STA & SDMMC_STA_CMDACT_Msk) && count-- != 0);
		SDMMC1->ICR |= 	SDMMC1->STA;
		if (count == 0) return False;
		if ((uint8_t)(SDMMC1->RESPCMD) != cmd) return False;
		// Response have been received
		if (SDMMC1->RESP1 & SD_CMD_RESP_R1_ERRORBITS) return False; // response have been received
	}

	return True;
}

static bool_t sdmmcCheckCmdResp2(uint8_t cmd)
{
	uint32_t count = SD_SEND_CMD_TIMEOUT_CNT;

	// Did not consider CCRCFAIL, CMDREND and CTIMEOUT error flags, because they will cause timeout.
	while ((SDMMC1->STA & SDMMC_STA_CMDACT_Msk) && count-- != 0);
	SDMMC1->ICR |= 	SDMMC1->STA;
	if (count == 0) return False;
	// Response have been received
	if (cmd == SD_CMD_ALL_SEND_CID) {
		sdcard.cid.raw[0] = SDMMC1->RESP1;
		sdcard.cid.raw[1] = SDMMC1->RESP2;
		sdcard.cid.raw[2] = SDMMC1->RESP3;
		sdcard.cid.raw[3] = SDMMC1->RESP4;
	}
	else if (cmd == SD_CMD_SEND_CSD) {
		sdcard.csd.raw[0] = SDMMC1->RESP1;
		sdcard.csd.raw[1] = SDMMC1->RESP2;
		sdcard.csd.raw[2] = SDMMC1->RESP3;
		sdcard.csd.raw[3] = SDMMC1->RESP4;
		sdcard.category = sdcard.csd.raw[2] >> 20;
	}

	return True;
}

static bool_t sdmmcCheckCmdResp3(void)
{
	uint32_t count = SD_SEND_CMD_TIMEOUT_CNT;

	// Did not consider CCRCFAIL, CMDREND and CTIMEOUT error flags, because they will cause timeout.
	while ((SDMMC1->STA & SDMMC_STA_CMDACT_Msk) && count-- != 0);
	SDMMC1->ICR |= 	SDMMC1->STA;
	if (count == 0) return False;

	return True;
}

static bool_t sdmmcCheckCmdResp6(uint8_t cmd)
{
	uint32_t count = SD_SEND_CMD_TIMEOUT_CNT;

	// Did not consider CCRCFAIL, CMDREND and CTIMEOUT error flags, because they will cause timeout.
	while ((SDMMC1->STA & SDMMC_STA_CMDACT_Msk) && count-- != 0);
	SDMMC1->ICR |= 	SDMMC1->STA;
	if (count == 0) return False;
	if ((uint8_t)(SDMMC1->RESPCMD) != cmd) return False;
	// Response have been received
	uint32_t r1 = SDMMC1->RESP1;
	if (r1 & SD_CMD_RESP_R6_ERRORBITS) return False;
	if (cmd == SD_CMD_SET_REL_ADDR) sdcard.rca = (uint16_t)(r1 >> 16); // CMD3

	return True;
}

static bool_t sdmmcCheckCmdResp7(void)
{
	uint32_t count = SD_SEND_CMD_TIMEOUT_CNT;

	// Did not consider CCRCFAIL, CMDREND and CTIMEOUT error flags, because they will cause timeout.
	while ((SDMMC1->STA & SDMMC_STA_CMDACT_Msk) && count-- != 0);
	SDMMC1->ICR |= 	SDMMC1->STA;
	if (count == 0) return False;

	return True;
}
