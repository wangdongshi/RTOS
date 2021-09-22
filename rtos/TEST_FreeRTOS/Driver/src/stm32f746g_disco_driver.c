/**********************************************************************
 * Copyright (c) 2018 - 2021 by Wang Yu
 * All rights reserved
 *
 * Filename:  stm32f746g_disco_driver.c
 * Project:   Minimum RTOS platform
 * Date:      2021/9/5
 * Author:    Wang Yu
 *
 **********************************************************************/
#include <stdint.h>
#include <string.h>
#include "stm32f746xx.h"
#include "stm32f746g_disco_driver.h"
#include "image.h"

// Clock constant value definition
#define PLLM					((uint32_t)( 25 <<  0))
#define PLLN					((uint32_t)(432 <<  6))
#define PLLP					((uint32_t)(  0 << 16)) // pay attention to this value!!
#define PLLSRC					((uint32_t)(  1 << 22))
#define PLLQ					((uint32_t)(  9 << 24))

// GPIO register setting pattern definition
#define GPIO_MODER_IN			(0b0)
#define GPIO_MODER_OUT			(0b1)
#define GPIO_MODER_MULTI		(0b10)
#define GPIO_MODER_SIM			(0b11)
#define GPIO_OTYPER_PP			(0b0)
#define GPIO_OTYPER_OD			(0b1)
#define GPIO_OSPEEDR_LOW		(0b00)
#define GPIO_OSPEEDR_MID		(0b01)
#define GPIO_OSPEEDR_HIGH		(0b10)
#define GPIO_OSPEEDR_FULL		(0b11)
#define GPIO_PUPDR_NONE			(0b00)
#define GPIO_PUPDR_UP			(0b01)
#define GPIO_PUPDR_DOWN			(0b10)
#define GPIO_PUPDR_RESERVE		(0b11)
#define GPIO_AFR_AF7			(0b0111)

// LCD constant value definition
#define LCD_FRAME_BUF_SIZE		(LCD_COLOR_BYTES * LCD_ACTIVE_WIDTH * LCD_ACTIVE_HEIGHT)
#define LCD_COLOR_BYTES			(3)
#define LCD_ACTIVE_WIDTH		(480)
#define LCD_ACTIVE_HEIGHT		(272)
#define LCD_HSYNC				(41)
#define LCD_VSYNC				(10)
#define LCD_HBP					(13)
#define LCD_HFP					(32)
#define LCD_VBP					(2)
#define LCD_VFP					(2)

uint8_t __attribute__( ( section(".sdram" ) ) ) __attribute__( ( aligned(4) ) ) FrameBuffer[LCD_FRAME_BUF_SIZE];

static uint32_t readRegister(uint32_t addr, uint32_t shift, uint32_t len);
static void writeRegister(uint32_t addr, uint32_t data, uint32_t shift, uint32_t len);
static void writeRegThenWait(uint32_t addr, uint32_t data, uint32_t shift, uint32_t len);
static void writeRegMask(uint32_t addr, uint32_t mask, uint32_t data);
static void writeRegMaskThenWait(uint32_t addr, uint32_t mask, uint32_t data);
static void waitValueSet(uint32_t addr, uint32_t mask, uint32_t data);
static void waitBitsSet(uint32_t addr, uint32_t mask);
static __attribute__((unused)) void waitBitsClear(uint32_t addr, uint32_t mask);

static void initFPU(void);
static void initSDRAM(void);
static void initLCD(void);
static void initI2C3(void);
static void initSystemClock(void);
static void initNVICPriorityGroup(void);
static void initSystickInt(void);
static void initSVCallInt(void);
static void initPendSVInt(void);
static void initSystick(void);
static void initUSART1Int(void);
static void initLED1(void);
static void initUSART1(void);
static void initUartGPIO(void);
static void initLED1GPIO(void);
static void initSDRAMGPIO(void);
static void initLCDGPIO(void);
static void initDMA2D(void);
static void initTouchPanelGPIO(void);

#ifdef MODE_STAND_ALONE
static void initTIM7Int(void);
static void initTIM7(void);
#endif

// Hardware driver API
void SystemInit(void)
{
	SCB_EnableICache();
	SCB_EnableDCache();
	initFPU();
	initSystemClock();
	initSDRAM();
	initNVICPriorityGroup();
	initSVCallInt();
	initPendSVInt();
	initLED1();
	initLCD();
	initUSART1();
	initI2C3();
	initSystick();
#ifdef MODE_STAND_ALONE
	initTIM7();
#endif
}

void toggleLED1(void)
{
	uint32_t data = readRegister(GPIOI_ODR, 1, 1);

	data = (!data) & 0x00000001;
	writeRegThenWait(GPIOI_ODR, data, 1, 1);
}

void usart1SendChar(const uint8_t character)
{
	waitBitsSet(USART1_ISR, ~0x00000040); // wait TC set
	writeRegThenWait(USART1_TDR, (uint32_t)character, 0, 8);
}

uint8_t usart1ReceiveChar(void)
{
	waitBitsSet(USART1_ISR, ~0x00000020); // wait RXNE set
	return (uint8_t)((readRegister(USART1_ISR, 0, 8) & 0x000000FF));
}

void usart1SendBuffer(const uint8_t* message)
{
	/*
	*((uint32_t *)USART1_CR1) |= 0x00000008; // enable TE
	for(uint32_t i = 0; i < strlen((const char*)message); i++) {
		*((uint32_t *)USART1_TDR) = (0x000000FF & message[i]); // set character to TX buffer
		while((*((uint32_t *)USART1_ISR) & 0x00000080) == 0); // wait TXE set
	}
	while((*((uint32_t *)USART1_ISR) & 0x00000040) == 0); // wait TC set
	*((uint32_t *)USART1_CR1) &= ~0x00000008; // disable TE
	*/
	writeRegThenWait(USART1_CR1, 1, 3, 1); // enable TE
	for(uint32_t i = 0; i < strlen((const char*)message); i++) {
		writeRegThenWait(USART1_TDR, message[i], 0, 8); // set character to TX buffer
		waitBitsSet(USART1_ISR, ~0x00000080); // wait TXE set
	}
	waitBitsSet(USART1_ISR, ~0x00000040); // wait TC set
	writeRegThenWait(USART1_CR1, 0, 3, 1); // disable TE
}

uint32_t getRandomData(void)
{
	RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;
	while((RCC->AHB2ENR & RCC_AHB2ENR_RNGEN_Msk) == 0);

	RNG->CR = RNG_CR_RNGEN;
	while((RNG->SR & RNG_SR_DRDY_Msk) == 0);

	uint32_t random = RNG->DR;

	RCC->AHB2RSTR |= RCC_AHB2RSTR_RNGRST;
	return random;
}

uint32_t testMemoryDMA(uint16_t data)
{
	uint32_t  size = 0x8000;
	uint16_t* pSrc = &data;
	uint16_t* pDes = ((uint16_t*)_SDRAM_BANK1) + (uint32_t)(0x700000/sizeof(uint16_t));

	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	while((RCC->AHB1ENR & RCC_AHB1ENR_DMA2EN_Msk) == 0);

	DMA2_Stream0->CR = 	DMA_SxCR_CHSEL_0 | // Stream0, channel1
						DMA_SxCR_PSIZE_0 | // src data 16bits
						DMA_SxCR_MSIZE_0 | // des data 16bits
						DMA_SxCR_MINC | 	// des address automatic increase
						DMA_SxCR_PL_0 | DMA_SxCR_PL_1 | // highest DMA priority
						DMA_SxCR_DIR_1; 	// memory to memory
	DMA2_Stream0->NDTR = size/sizeof(uint16_t);	 // 32768 / 2 = 16 K half word
	DMA2_Stream0->PAR  = (uint32_t)(pSrc);
	DMA2_Stream0->M0AR = (uint32_t)(pDes);

	DMA2_Stream0->CR |= DMA_SxCR_EN; // start DMA transfer

	while(DMA2_Stream0->NDTR != 0);  // wait transfer complete
	DMA2->LIFCR |= DMA_LIFCR_CTCIF0; // must clear TC interrupt flag for next DMA

	RCC->AHB1RSTR |= RCC_AHB1RSTR_DMA2RST;

	return (*((uint16_t*)0xC0700000) == data && *((uint16_t*)0xC0700000 + 0x3FF8) == data);
}

void showLogo(void)
{
	uint32_t size = LCD_FRAME_BUF_SIZE / sizeof(uint32_t) / 2; // it must lower than 65535

	// Open DMA RRC
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	while((RCC->AHB1ENR & RCC_AHB1ENR_DMA2EN_Msk) == 0);

	// RGB888 above half frame (48960 uint32_t)
	DMA2_Stream0->CR = 	DMA_SxCR_CHSEL_0 |	// Stream0, channel1
						DMA_SxCR_PSIZE_1 |	// src data 32bits
						DMA_SxCR_MSIZE_1 |	// des data 32bits
						DMA_SxCR_PINC |		// src address automatic increase
						DMA_SxCR_MINC |		// des address automatic increase
						DMA_SxCR_PL_0 | DMA_SxCR_PL_1 | // highest DMA priority
						DMA_SxCR_DIR_1; 	// memory to memory
	DMA2_Stream0->NDTR = size;
	DMA2_Stream0->PAR  = (uint32_t)&logoImage;
	DMA2_Stream0->M0AR = (uint32_t)&FrameBuffer;
	DMA2_Stream0->CR |= DMA_SxCR_EN;  // start DMA transfer
	while((DMA2->LISR & DMA_LISR_TCIF0_Msk) == 0);   // wait transfer complete
	DMA2->LIFCR |= DMA_LIFCR_CTCIF0;  // must clear TC interrupt flag for next DMA

	// RGB888 below half frame (48960 uint32_t)
	DMA2_Stream0->CR = 	DMA_SxCR_CHSEL_0 |	// Stream0, channel1
						DMA_SxCR_PSIZE_1 |	// src data 32bits
						DMA_SxCR_MSIZE_1 |	// des data 32bits
						DMA_SxCR_PINC |		// src address automatic increase
						DMA_SxCR_MINC |		// des address automatic increase
						DMA_SxCR_PL_0 | DMA_SxCR_PL_1 | // highest DMA priority
						DMA_SxCR_DIR_1; 	// memory to memory
	DMA2_Stream0->NDTR = size;
	DMA2_Stream0->PAR  = (uint32_t)((uint32_t*)&logoImage + size);
	DMA2_Stream0->M0AR = (uint32_t)((uint32_t*)&FrameBuffer + size);
	DMA2_Stream0->CR |= DMA_SxCR_EN; // start DMA transfer
	while((DMA2->LISR & DMA_LISR_TCIF0_Msk) == 0);   // wait transfer complete
	DMA2->LIFCR |= DMA_LIFCR_CTCIF0; // must clear TC interrupt flag for next DMA

	// close DMA RRC
	RCC->AHB1RSTR |= RCC_AHB1RSTR_DMA2RST;
}

void FillRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color)
{
	DMA2D->CR 		&= ~DMA2D_CR_MODE_Msk;
	DMA2D->CR 		|= 0b11 << DMA2D_CR_MODE_Pos; // register to memory
	DMA2D->OCOLR	= color;
	DMA2D->OMAR		= (uint32_t)(&(((uint8_t*)&FrameBuffer)[(y * LCD_ACTIVE_WIDTH + x) * LCD_COLOR_BYTES]));
	DMA2D->OOR		= LCD_ACTIVE_WIDTH - w;
	DMA2D->OPFCCR	= 0b01;
	DMA2D->NLR		= w << DMA2D_NLR_PL_Pos | h << DMA2D_NLR_NL_Pos;

	DMA2D->CR   	|= DMA2D_CR_START;
	while (DMA2D->CR & DMA2D_CR_START);
}

// General register(4 bytes) operation function
static uint32_t readRegister(uint32_t addr, uint32_t shift, uint32_t len)
{
	uint32_t* reg = (uint32_t*)addr;
	uint32_t  val = *reg; // read from register
	uint32_t  mask = 0x00000000;

	// set mask
	for (uint32_t i = 0; i < 32; i++) {
		if (i >= shift && i < shift + len)
			mask |= (1 << i);
	}
	mask = ~mask;

	// set data
	val = (val & ~mask) >> shift;
	return val;
}

static void writeRegister(uint32_t addr, uint32_t data, uint32_t shift, uint32_t len)
{
	uint32_t* reg = (uint32_t*)addr;
	uint32_t  val = *reg; // read from register
	uint32_t  mask = 0x00000000;

	// set mask
	for (uint32_t i = 0; i < 32; i++) {
		if (i >= shift && i < shift + len)
			mask |= (1 << i);
	}
	mask = ~mask;

	// set data
	val &= mask;					// clear bits
	val |= (data << shift) & ~mask; // set bits
	*reg = val;						// write back to register
}

static void writeRegThenWait(uint32_t addr, uint32_t data, uint32_t shift, uint32_t len)
{
	uint32_t* reg = (uint32_t*)addr;
	uint32_t  val = *reg; // read from register
	uint32_t  mask = 0x00000000;

	// set mask
	for (uint32_t i = 0; i < 32; i++) {
		if (i >= shift && i < shift + len)
			mask |= (1 << i);
	}
	mask = ~mask;

	// set data
	val &= mask;					// clear bits
	val |= (data << shift) & ~mask; // set bits
	*reg = val;						// write back to register
	while(*reg != val);				// wait for set complete
}

static void writeRegMask(uint32_t addr, uint32_t mask, uint32_t data)
{
	volatile uint32_t* reg = (uint32_t*)addr;
	uint32_t val = *reg; 	// read from register

	// set data
	val &= mask;			// clear bits
	val |= data & (~mask);	// set bits
	*reg = val;				// write back to register
}

static void writeRegMaskThenWait(uint32_t addr, uint32_t mask, uint32_t data)
{
	volatile uint32_t* reg = (uint32_t*)addr;
	uint32_t val = *reg; 	// read from register

	// set data
	val &= mask;			// clear bits
	val |= data & (~mask);	// set bits
	*reg = val;				// write back to register
	while(*reg != val);		// wait for set complete
}

static void waitValueSet(uint32_t addr, uint32_t mask, uint32_t data)
{
	volatile uint32_t* reg = (uint32_t*)addr;
	while((*reg & ~mask) != data);

}

static void waitBitsSet(uint32_t addr, uint32_t mask)
{
	volatile uint32_t* reg = (uint32_t*)addr;
	while((*reg & ~mask) == 0);
}

static void waitBitsClear(uint32_t addr, uint32_t mask)
{
	volatile uint32_t* reg = (uint32_t*)addr;
	while((*reg & ~mask) != 0);
}

// The following initialization process is write for STM32F746G-DISCO
// FPU initialization
static void initFPU(void)
{
	writeRegThenWait(SCR_CPACR, 0b1111, 20, 4); // open CP10 & CP11
}

// System clock initialization
static void initSystemClock(void)
{
	// 1. Set HSE and reset RCC_CIR
	writeRegThenWait(RCC_CR, 0b1, 18, 1); // set HSEBYP bit
	writeRegThenWait(RCC_CR, 0b1, 16, 1); // set HSEON bit
	waitBitsSet(RCC_CR, ~0x00020000); // wait HSERDY set
	writeRegThenWait(RCC_CIR, 0x00000000, 0, 32); // disable all RCC interrupts

	// 2. Set FLASH latency
	writeRegThenWait(FLASH_ACR, 7, 0, 4); // need be set in 216MHz
	//writeRegThenWait(FLASH_ACR, 0b11, 8, 2); // set ARTEN, PRFTEN
	//writeRegThenWait(PWR_CR1, 0b11, 14, 2); // PWR_VOS default value has set to 0b11 in reset

	// 3. Enable PWR clock
	writeRegThenWait(RCC_APB1ENR, 0b1, 28, 1);
	//*((uint32_t *)RCC_APB1ENR) |= 0x10000000;

	// 4. Activation OverDrive Mode
	writeRegThenWait(PWR_CR1, 0b1, 16, 1); // set ODEN
	waitBitsSet(PWR_CSR1, ~0x00010000); // wait ODRDY

	// 5. Activation OverDrive Switching
	writeRegThenWait(PWR_CR1, 0b1, 17, 1); // set ODEN
	waitBitsSet(PWR_CSR1, ~0x00020000); // wait ODRDY

	// 6. Main PLL configuration and activation
	writeRegThenWait(RCC_PLLCFGR, (PLLM | PLLN | PLLP | PLLSRC | PLLQ), 0, 32); // reset PLLCFGR register
	writeRegThenWait(RCC_CR, 0b1, 24, 1); // set PLLON
	waitBitsSet(RCC_CR, ~0x02000000); // wait for PLLRDY

	// 7. System clock activation on the main PLL
	writeRegThenWait(RCC_CFGR, 0b0000, 4, 4); // set HPRE  (AHB  pre-scaler) to 1 (216 MHz)
	writeRegThenWait(RCC_CFGR, 0b101, 10, 3); // set PPRE1 (APB1 pre-scaler) to 4 (54  MHz)
	writeRegThenWait(RCC_CFGR, 0b100, 13, 3); // set PPRE2 (APB2 pre-scaler) to 2 (108 MHz)
	waitValueSet(RCC_CFGR, ~0x0000FC00, 0x00009400); // Attention : before switching System clock to PLL, it must wait all pre-scale value set to CFGR.
	writeRegister(RCC_CFGR, 0b10, 0, 2); // set SW to PLL
	waitValueSet(RCC_CFGR, ~0x0000000C, 0x00000008); // wait for system clock set to PLL
}

// Set global NVIC priority group
static void initNVICPriorityGroup(void)
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

	// "|=" can not be used here, because VECTKEY write key is "0x05FA" and "|=" will change it
	writeRegister(SCR_AIRCR, 0x05FA0300, 0, 32); // PRIGROUP = 0b011 (preemption:16, sub:0)
	waitValueSet(SCR_AIRCR, ~0x00000700, 0x00000300);

	// Global interrupt design in sample project can refer to the following setting.
	// SVC(FreeRTOS)		: preemption:10, IRQn:11
	// PendSV(FreeRTOS)		: preemption:15, IRQn:14
	// Systick(FreeRTOS)	: preemption:6,  IRQn:15
	// USART1(Debug)		: preemption:14, IRQn:37
	// TIMER7(LED1 flicker)	: preemption:14, IRQn:55
}

// Initialize USART1 interrupt
static void initUSART1Int(void)
{
	// Set USART1 interrupt priority
	writeRegThenWait(NVIC_IPR9, 14, (8+4), 4);

	// Enable USART1 global interrupt (IRQn=37)
	writeRegThenWait(NVIC_ISER1, 0b1, (37-32), 1);
}

// Initialize SVCall interrupt (IRQn:11)
static void initSVCallInt(void)
{
	// Set SVCall interrupt priority
	writeRegThenWait(SCR_SHPR2, 10, (24+4), 4);

	// SVC need not open. Executing SVC instrument will trigger SVC interrupt.
}

// Initialize PendSV interrupt (IRQn:14)
static void initPendSVInt(void)
{
	// Set PendSV interrupt priority
	writeRegThenWait(SCR_SHPR3, 15, (16+4), 4);

	// PendSV need not open. Setting ICSR the 28th bit will trigger pendSV interrupt.
}

// Initialize Systick interrupt (IRQn:15)
static void initSystickInt(void)
{
	// Set Systick interrupt priority
	writeRegThenWait(SCR_SHPR3, 6, (24+4), 4);

	// Enable Systick global interrupt
	writeRegThenWait(SYST_CSR, 0b1, 1, 1);
}

// LED1 initialization
static void initLED1(void)
{
	initLED1GPIO();
}

// USART1 initialization
static void initUSART1(void)
{
	// Initialize USART1 TX/RX pin
	initUartGPIO();

	// Enable APB2 USART1 RCC
	writeRegThenWait(RCC_APB2ENR, 0b1, 4, 1);

	// Set USART1 parameter
	//writeRegThenWait(USART1_BRR, 0x2BF2, 0, 16); // baud rate = 9600(fCK=108MHz, CR1/OVER8=0, 0x2BF2 = 108000000/9600)
	writeRegThenWait(USART1_BRR, 0x03AA, 0, 16); // baud rate = 115200(fCK=108MHz, CR1/OVER8=0, 0x03AA = 108000000/115200)
	writeRegMaskThenWait(USART1_CR1, 0x10001000, 0x00000000);	// data bits = 8(M[1:0]=00)
	writeRegThenWait(USART1_CR2, 0b00, 12, 2);					// stop bits = 1(STOP[1:0]=00)
	writeRegThenWait(USART1_CR1, 0b0, 10, 1);					// parity = off(parity = on, odd:0x00000600, even:0x00000400)
	writeRegThenWait(USART1_CR3, 0b00, 8, 2);					// disable CTS and RTS

	// Enable USART1 global interrupt
	initUSART1Int();

	// Disable RX interrupt in console log mode
	//writeRegThenWait(USART1_CR1, 0b1, 5, 1); // set RXNEIE

	// Enable USART1
	writeRegThenWait(USART1_CR1, 0b1, 0, 1);

	// Enable TX and RE
	writeRegThenWait(USART1_CR1, 0b11, 2, 2);
}

// System tick initialization
static void initSystick(void)
{
	// Enable Systick global interrupt
	initSystickInt();

	// Set RELOAD value (216000000 / 1000 = 216000)
	writeRegThenWait(SYST_RVR, (0x00034BC0-1), 0, 32);

	// Set current value by default value
	//writeRegThenWait(SYST_CVR, 0x00000000, 0, 32);

	// Set CLKSOURCE, ENABLE
	writeRegThenWait(SYST_CSR, 0b1, 2, 1); // clk-source : processor clock
	writeRegThenWait(SYST_CSR, 0b1, 0, 1); // enable systick counter
}

#ifdef MODE_STAND_ALONE
// Initialize TIM7 interrupt
static void initTIM7Int(void)
{
	// Set TIM7 interrupt priority for LED1 flicker
	writeRegThenWait(NVIC_IPR13, 14, (24+4), 4);

	// Enable TIM7 global interrupt (IRQn=55)
	writeRegThenWait(NVIC_ISER1, 0b1, (55-32), 4);

	// Enable TIM7 update interrupt
	writeRegThenWait(TIM7_DIER, 0b1, 0, 1);
}

// TIM7 initialization for low speed timer(500ms)
static void initTIM7(void)
{
	// Enable APB1 TIM7 RCC
	writeRegThenWait(RCC_APB1ENR, 0b1, 5, 1);
	
	// Enable TIM7 interrupt
	initTIM7Int();

	// Enable pre-load ARPE
	writeRegThenWait(TIM7_CR1, 0b1, 7, 1);
	
	// Set 4000 to pre-scale
	writeRegThenWait(TIM7_PSC, (4000-1), 0, 16); // timer frequency = 108MHz / 4000 = 27KHz (APB pre-sacle is not 1, so fCK_PSC = 54MHz * 2 = 108MHz)
	
	// Set 0 to CNT
	//writeRegThenWait(TIM7_CNT, 0, 0, 16); // reset CNT

	// Clear timer update interrupt flag
	writeRegThenWait(TIM7_SR, 0b0, 0, 1); // reset UIF
	
	// Set 13500 to reload value
	writeRegThenWait(TIM7_ARR, (13500-1), 0, 16);
	
	// Enable TIM7 CEN
	writeRegThenWait(TIM7_CR1, 0b1, 0, 1);
}
#endif

// USART TX pin initialization (TX:GPIOA9, RX:GPIOB7)
static void initUartGPIO(void)
{
	/*
	writeRegThenWait(RCC_AHB1ENR, 1, 0, 1);
	writeRegThenWait(GPIOA_MODER,	GPIO_MODER_MULTI,	9*2,		2);
	writeRegThenWait(GPIOA_OTYPER,	GPIO_OTYPER_PP,		9,			1);
	writeRegThenWait(GPIOA_OSPEEDR,	GPIO_OSPEEDR_FULL,	9*2,		2);
	writeRegThenWait(GPIOA_PUPDR,	GPIO_PUPDR_UP,		9*2,		2);
	writeRegThenWait(GPIOA_AFRH,	GPIO_AFR_AF7,		(9-8)*4,	4);

	writeRegThenWait(RCC_AHB1ENR, 1, 1, 1);
	writeRegThenWait(GPIOB_MODER,	GPIO_MODER_MULTI,	7*2,		2);
	writeRegThenWait(GPIOB_OTYPER,	GPIO_OTYPER_PP,		7,			1);
	writeRegThenWait(GPIOB_AFRL,	GPIO_AFR_AF7,		7*4,		4);
	*/
	writeRegMask(RCC_AHB1ENR,	~0x00000001,	0x00000001);
	writeRegMask(GPIOA_MODER,	~0x000C0000,	0x00080000);
	writeRegMask(GPIOA_OTYPER,	~0x00000200,	0x00000000);
	writeRegMask(GPIOA_OSPEEDR,	~0x000C0000,	0x000C0000);
	writeRegMask(GPIOA_PUPDR,	~0x000C0000,	0x00040000);
	writeRegMask(GPIOA_AFRH,	~0x000000F0,	0x00000070);

	writeRegMask(RCC_AHB1ENR,	~0x00000002,	0x00000002);
	writeRegMask(GPIOB_MODER,	~0x000C0000,	0x00080000);
	writeRegMask(GPIOB_OTYPER,	~0x00000200,	0x00000000);
	// RX line need not to set OSPEEDR and PUPDR
	writeRegMask(GPIOB_AFRL,	~0xF0000000,	0x70000000);
}

// LED1 pin initialization (GPIOI1)
static void initLED1GPIO(void)
{
	writeRegThenWait(RCC_AHB1ENR,	0b1, 8, 1);
	writeRegThenWait(GPIOI_MODER,	GPIO_MODER_OUT,		1*2,		2);
	writeRegThenWait(GPIOI_OTYPER,	GPIO_OTYPER_PP,		1,			1);
	writeRegThenWait(GPIOI_OSPEEDR,	GPIO_OSPEEDR_FULL,	1*2,		2);
	writeRegThenWait(GPIOI_PUPDR,	GPIO_PUPDR_UP,		1*2,		2);

}

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
	// PC3  --> FMC_SDCKE0
	writeRegMask(RCC_AHB1ENR,		~0x00000004,	0x00000004);
	writeRegMask(GPIO_MODER(C),		~0x000000C0,	0x00000080); // MODER = 10(Multiple)
	writeRegMask(GPIO_OTYPER(C),	~0x00000010,	0x00000000); // OTYPER = PP
	writeRegMask(GPIO_OSPEEDR(C),	~0x000000C0,	0x000000C0); // OSPEEDR = Full
	writeRegMask(GPIO_PUPDR(C),		~0x000000C0,	0x00000000); // PUPDR = No Pull
	writeRegMask(GPIO_AFRL(C),		~0x0000F000,	0x0000C000); // AF = AF12

	// GPIOD
	// PD0  --> FMC_D2
	// PD1  --> FMC_D3
	// PD8  --> FMC_D13
	// PD9  --> FMC_D14
	// PD10 --> FMC_D15
	// PD14 --> FMC_D0
	// PD15 --> FMC_D1
	writeRegMask(RCC_AHB1ENR,		~0x00000008,	0x00000008);
	writeRegMask(GPIO_MODER(D),		~0xF03F000F,	0xA02A000A); // MODER = 10(Multiple)
	writeRegMask(GPIO_OTYPER(D),	~0x0000C703,	0x00000000); // OTYPER = PP
	writeRegMask(GPIO_OSPEEDR(D),	~0xF03F000F,	0xF03F000F); // OSPEEDR = Full
	writeRegMask(GPIO_PUPDR(D),		~0xF03F000F,	0x00000000); // PUPDR = No Pull
	writeRegMask(GPIO_AFRL(D),		~0x000000FF,	0x000000CC); // AF = AF12
	writeRegMask(GPIO_AFRH(D),		~0xFF000FFF,	0xCC000CCC); // AF = AF12

	// GPIOE
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
	writeRegMask(RCC_AHB1ENR,		~0x00000010,	0x00000010);
	writeRegMask(GPIO_MODER(E),		~0xFFFFC00F,	0xAAAA800A); // MODER = 10(Multiple)
	writeRegMask(GPIO_OTYPER(E),	~0x0000FF83,	0x00000000); // OTYPER = PP
	writeRegMask(GPIO_OSPEEDR(E),	~0xFFFFC00F,	0xFFFFC00F); // OSPEEDR = Full
	writeRegMask(GPIO_PUPDR(E),		~0xFFFFC00F,	0x00000000); // PUPDR = No Pull
	writeRegMask(GPIO_AFRL(E),		~0xF00000FF,	0xC00000CC); // AF = AF12
	writeRegMask(GPIO_AFRH(E),		~0xFFFFFFFF,	0xCCCCCCCC); // AF = AF12

	// GPIOF
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
	writeRegMask(RCC_AHB1ENR,		~0x00000020,	0x00000020);
	writeRegMask(GPIO_MODER(F),		~0xFFC00FFF,	0xAA800AAA); // MODER = 10(Multiple)
	writeRegMask(GPIO_OTYPER(F),	~0x0000F83F,	0x00000000); // OTYPER = PP
	writeRegMask(GPIO_OSPEEDR(F),	~0xFFC00FFF,	0xFFC00FFF); // OSPEEDR = Full
	writeRegMask(GPIO_PUPDR(F),		~0xFFC00FFF,	0x00000000); // PUPDR = No Pull
	writeRegMask(GPIO_AFRL(F),		~0x00FFFFFF,	0x00CCCCCC); // AF = AF12
	writeRegMask(GPIO_AFRH(F),		~0xFFFFF000,	0xCCCCC000); // AF = AF12

	// GPIOG
	// PG0  --> FMC_A10
	// PG1  --> FMC_A11
	// PG4  --> FMC_BA0
	// PG5  --> FMC_BA1
	// PG8  --> FMC_SDCLK
	// PG15 --> FMC_SDNCAS
	writeRegMask(RCC_AHB1ENR,		~0x00000040,	0x00000040);
	writeRegMask(GPIO_MODER(G),		~0xC0030F0F,	0x80020A0A); // MODER = 10(Multiple)
	writeRegMask(GPIO_OTYPER(G),	~0x00008133,	0x00000000); // OTYPER = PP
	writeRegMask(GPIO_OSPEEDR(G),	~0xC0030F0F,	0xC0030F0F); // OSPEEDR = Full
	writeRegMask(GPIO_PUPDR(G),		~0xC0030F0F,	0x00000000); // PUPDR = No Pull
	writeRegMask(GPIO_AFRL(G),		~0x00FF00FF,	0x00CC00CC); // AF = AF12
	writeRegMask(GPIO_AFRH(G),		~0xF000000F,	0xC000000C); // AF = AF12

	// GPIOH
	// PH3  --> FMC_SDNE0
	// PH5  --> FMC_SDNWE
	writeRegMask(RCC_AHB1ENR,		~0x00000080,	0x00000080);
	writeRegMask(GPIO_MODER(H),		~0x00000CC0,	0x00000880); // MODER = 10(Multiple)
	writeRegMask(GPIO_OTYPER(H),	~0x00000028,	0x00000000); // OTYPER = PP
	writeRegMask(GPIO_OSPEEDR(H),	~0x00000CC0,	0x00000CC0); // OSPEEDR = Full
	writeRegMask(GPIO_PUPDR(H),		~0x00000CC0,	0x00000000); // PUPDR = No Pull
	writeRegMask(GPIO_AFRL(H),		~0x00F0F000,	0x00C0C000); // AF = AF12
}

static void initDMA2D(void)
{
	RCC->AHB1ENR	|= RCC_AHB1ENR_DMA2DEN;
	while((RCC->AHB1ENR & RCC_AHB1ENR_DMA2DEN_Msk) == 0);
}

// LCD pin initialization
static void initLCDGPIO(void)
{
	// GPIOE
	RCC->AHB1ENR	|= RCC_AHB1ENR_GPIOEEN;
	while((RCC->AHB1ENR & RCC_AHB1ENR_GPIOEEN_Msk) == 0);
	// PE4  --> LTDC_B0
	GPIOE->MODER	|=	0b10	<< GPIO_MODER_MODER4_Pos;		// MODER = Multiple(0b10)
	GPIOE->AFR[0]	|=	14		<< GPIO_AFRL_AFRL4_Pos;			// AF = AF14

	// GPIOG
	RCC->AHB1ENR	|=	RCC_AHB1ENR_GPIOGEN;
	while((RCC->AHB1ENR & RCC_AHB1ENR_GPIOGEN_Msk) == 0);
	// PG12 --> LTDC_B4
	GPIOG->MODER	|=	0b10	<< GPIO_MODER_MODER12_Pos;		// MODER = Multiple(0b10)
	GPIOG->AFR[1]	|=	14		<< GPIO_AFRH_AFRH4_Pos;			// AF = AF14

	// GPIOI
	RCC->AHB1ENR	|=	RCC_AHB1ENR_GPIOIEN;
	while((RCC->AHB1ENR & RCC_AHB1ENR_GPIOIEN_Msk) == 0);
	// PI9  --> LTDC_VSYNC
	// PI10 --> LTDC_HSYNC
	// PI14 --> LTDC_CLK
	// PI15 --> LTDC_R0
	GPIOI->MODER	|=	0b10	<< GPIO_MODER_MODER9_Pos |		// MODER = Multiple(0b10)
						0b10	<< GPIO_MODER_MODER10_Pos |
						0b10	<< GPIO_MODER_MODER14_Pos |
						0b10	<< GPIO_MODER_MODER15_Pos;
	GPIOI->AFR[1]	|=	14		<< GPIO_AFRH_AFRH1_Pos |		// AF = AF14
						14		<< GPIO_AFRH_AFRH2_Pos |
						14		<< GPIO_AFRH_AFRH6_Pos |
						14		<< GPIO_AFRH_AFRH7_Pos;

	// GPIOJ
	RCC->AHB1ENR	|=	RCC_AHB1ENR_GPIOJEN;
	while((RCC->AHB1ENR & RCC_AHB1ENR_GPIOJEN_Msk) == 0);
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
	GPIOJ->AFR[0]	|=	0xEEEEEEEE; // AF = AF14
	GPIOJ->AFR[1]	|=	0xEEE0EEEE; // AF = AF14

	// GPIOK
	RCC->AHB1ENR	|=	RCC_AHB1ENR_GPIOKEN;
	while((RCC->AHB1ENR & RCC_AHB1ENR_GPIOKEN_Msk) == 0);
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
	while((RCC->AHB1ENR & RCC_AHB1ENR_GPIOHEN_Msk) == 0);
	// PH7  --> I2C3_SCL(for FT5336 Control)
	// PH8  --> I2C3_SDA
	GPIOH->MODER	|=	0b10	<< GPIO_MODER_MODER7_Pos |	// MODER = Multiple(0b10)
						0b10	<< GPIO_MODER_MODER8_Pos;
	GPIOH->OTYPER	|=	0b1		<< GPIO_OTYPER_OT7_Pos;		// Open Drain
	GPIOH->OSPEEDR	|=	0b10	<< GPIO_OSPEEDR_OSPEEDR7_Pos; // High speed
	GPIOH->PUPDR	|=	0b01	<< GPIO_PUPDR_PUPDR7_Pos;	// Pull-up
	GPIOH->AFR[0]	|=	4		<< GPIO_AFRL_AFRL7_Pos;		// AF = AF4
	GPIOH->AFR[1]	|=	4		<< GPIO_AFRH_AFRH0_Pos;		// AF = AF4

	// GPIOI RCC
	RCC->AHB1ENR	|=	RCC_AHB1ENR_GPIOIEN;
	while((RCC->AHB1ENR & RCC_AHB1ENR_GPIOIEN_Msk) == 0);
	// PI13 --> LCD_INT
	GPIOI->MODER	|=	0b10	<< GPIO_MODER_MODER15_Pos;	// MODER = Multiple(0b10)
	GPIOI->AFR[1]	|=	15		<< GPIO_AFRH_AFRH5_Pos;		// AF = AF15
}

// I2C3 controller initialization (for touch panel)
static void initI2C3(void)
{
	// Initialize GPIO for I2C3
	initTouchPanelGPIO();

	// Enable I2C3 RCC
	RCC->APB1ENR	|=	RCC_APB1ENR_I2C3EN;
	while((RCC->APB1ENR & RCC_APB1ENR_I2C3EN_Msk) == 0);

	// Set I2C3 clock
	RCC->DCKCFGR2	&=	~RCC_DCKCFGR2_I2C3SEL_Msk;
	RCC->DCKCFGR2	|=	0b00 << RCC_DCKCFGR2_I2C3SEL_Pos; // APB1 clock (54MHz)

	// Set I2C3 register
	I2C3->CR1		&=	~I2C_CR1_PECEN_Msk;
	I2C3->TIMINGR	=	0x30C03444; // from CubeMX (I2C Frequency = 100KHz, Rise time = 700ns, Fall time = 100ns)
	I2C3->OAR1		|=	0x30F	<< I2C_OAR1_OA1_Pos |
						0b1		<< I2C_OAR1_OA1MODE_Pos |
						0b1		<< I2C_OAR1_OA1EN_Pos;

	// Reset I2C3
	while(I2C3->CR1 & I2C_CR1_PECEN_Msk);
	I2C3->CR1		|=	I2C_CR1_PECEN;
}

// for 7bits slave address and 8bits memory address
void i2c3Write1Byte(uint8_t slaveAddr, uint8_t devAddr, uint8_t data)
{
	// Wait previous I2C operation complete
	while (I2C3->ISR & I2C_ISR_BUSY);

	// Master mode communication initialization
	I2C3->CR2	=	0x00000000;
	I2C3->CR2	=	I2C_CR2_AUTOEND |
					2 << I2C_CR2_NBYTES_Pos | // send 2 bytes (address + data)
					I2C_CR2_NACK |
					I2C_CR2_START |
					I2C_CR2_ADD10 |
					0b0 << I2C_CR2_RD_WRN_Pos | // write
					slaveAddr << I2C_CR2_SADD_Pos;

	// Send device address and data
	while ((I2C3->ISR & I2C_ISR_TXIS_Msk) == 0);
	I2C3->TXDR	=	devAddr;
	while ((I2C3->ISR & I2C_ISR_TXIS_Msk) == 0);
	I2C3->TXDR	=	data;

	// Check and clean stop bit
	while ((I2C3->ISR & I2C_ISR_STOPF_Msk) == 0);
	I2C3->ICR	|=	I2C_ICR_STOPCF;
}

// for 7bits slave address and 8bits memory address
uint8_t i2c3Read1Byte(uint8_t slaveAddr, uint8_t devAddr)
{
	uint8_t data;

	// Wait previous I2C operation complete
	while (I2C3->ISR & I2C_ISR_BUSY);

	// Master mode communication initialization
	I2C3->CR2	=	0x00000000;
	I2C3->CR2	=	//I2C_CR2_AUTOEND |
					1 << I2C_CR2_NBYTES_Pos | // send device address
					//I2C_CR2_NACK |
					//I2C_CR2_START |
					//I2C_CR2_ADD10 |
					0b0 << I2C_CR2_RD_WRN_Pos | // write
					slaveAddr << I2C_CR2_SADD_Pos;

	I2C3->CR2	|=	I2C_CR2_START;

	// Send device address
	//while ((I2C3->ISR & I2C_ISR_TXIS_Msk) == 0);
	I2C3->TXDR	=	devAddr;

	// Check and clean stop bit
	while ((I2C3->ISR & I2C_ISR_STOPF_Msk) == 0);
	I2C3->ICR	|=	I2C_ICR_STOPCF;

	// Wait previous I2C operation complete
	while (I2C3->ISR & I2C_ISR_BUSY);

	// Master mode communication initialization
	I2C3->CR2	=	0x00000000;
	I2C3->CR2	=	I2C_CR2_AUTOEND |
					1 << I2C_CR2_NBYTES_Pos | // receive data
					I2C_CR2_NACK |
					I2C_CR2_START |
					I2C_CR2_ADD10 |
					0b1 << I2C_CR2_RD_WRN_Pos | // read
					slaveAddr << I2C_CR2_SADD_Pos;

	// Wait receive data prepared
	while ((I2C3->ISR & I2C_ISR_RXNE_Msk) == 0);
	data		=	(uint8_t)I2C3->RXDR;

	// Check and clean stop bit
	while ((I2C3->ISR & I2C_ISR_STOPF_Msk) == 0);
	I2C3->ICR	|=	I2C_ICR_STOPCF;

	return data;
}

// SDRAM initialization
static void initSDRAM(void)
{
	// Initialize GPIO for FMC
	initSDRAMGPIO();

	// Enable FMC RCC
	writeRegMaskThenWait(RCC_AHB3ENR, ~0x00000001, 0x00000001);

	// The following comment is copied from STM32F746G data-sheet FMC-SDRAM chapter.
	// Set MCU-FMC register
	// 1. Program the memory device features into the FMC_SDCRx register.
	//    The SDRAM clock frequency, RBURST and RPIPE must be programmed in
	//    the FMC_SDCR1 register.
	uint32_t NC     = 0b00 << 0;	// 8 bits
	uint32_t NR     = 0b01 << 2;	// 12 bits
	uint32_t NWID   = 0b01 << 4;	// 16 bits
	uint32_t NB     = 0b1 << 6;		// 4 banks
	uint32_t CAS    = 0b11 << 7;	// 3 cycles
	uint32_t WP     = 0b0 << 9;		// enable write operation
	uint32_t SDCLK  = 0b10 << 10;	// HCLK x 2 = 108MHz = 9.26ns
	uint32_t RBURST = 0b1 << 12;	// enable burst mode
	uint32_t RPIPE  = 0b0 << 13;	// 0 latency after CAS
	uint32_t SDCR   = NC | NR | NWID | NB | CAS | WP | SDCLK | RBURST | RPIPE;
	writeRegMaskThenWait(FMC_SDCR1, ~0x00007FFF, SDCR);

	// 2. Program the memory device timing into the FMC_SDTRx register.
	//    The TRP and TRC timings must be programmed in the FMC_SDTR1 register.
	uint32_t TMRD	= 1 << 0;		// 1 cycle : uncertain !
	uint32_t TXSR	= 8 << 4;		// 8 x 9.26 = 74.08ns > tXSR(70ns)
	uint32_t TRAS	= 3 << 8;		// 3 x 9.26 = 27.78ns < tRFC(70ns) : uncertain !
	uint32_t TRC	= 8 << 12;		// 8 x 9.26 = 74.08ns > tRC(70ns)
	uint32_t TWR	= 3 << 16;		// 3 x 9.26 = 27.78ns > tWR(1CLK+7ns or 14ns) : uncertain !
	uint32_t TRP	= 3 << 20;		// 3 x 9.26 = 27.78ns > tRP(20ns)
	uint32_t TRCD	= 3 << 24;		// 3 x 9.26 = 27.78ns < tRCD(20ns)
	uint32_t SDTR	= TMRD | TXSR | TRAS | TRC | TWR | TRP | TRCD;
	writeRegMaskThenWait(FMC_SDTR1, ~0x0FFFFFFF, SDTR);

	// Set SDRAM-MODE register
	uint32_t MODE, CTB1, NRFS, MRD, SDCMR;
	// 3. Set MODE bits to ‘001’ and configure the Target Bank bits
	//    (CTB1 and/or CTB2) in the	FMC_SDCMR register to start delivering
	//    the clock to the memory (SDCKE is driven high).
	MODE	= 0b001 << 0;	// clock assign enable
	CTB1	= 0b1 << 4;		// CTB1 enable
	NRFS	= 0 << 5;		// not use
	MRD		= 0 << 9;		// not use
	SDCMR	= MODE | CTB1 | NRFS | MRD;
	writeRegMask(FMC_SDCMR, ~0x003FFFFF, SDCMR);

	// 4. Wait during the prescribed delay period. Typical delay is around
	//    100 us (refer to the SDRAM datasheet for the required delay after power-up).
	for (volatile uint32_t i = 0; i < 50000; i++); // at least 100 us delay here, (216MHz = 4.63ns, 100us / 2.63ns = 21598)

	// 5. Set MODE bits to ‘010’ and configure the Target Bank bits
	//    (CTB1 and/or CTB2) in the FMC_SDCMR register to issue a “Precharge All” command.
	MODE	= 0b010 << 0;	// pre-charge all
	CTB1	= 0b1 << 4;		// CTB1 enable
	NRFS	= 0 << 5;		// not use
	MRD		= 0 << 9;		// not use
	SDCMR	= MODE | CTB1 | NRFS | MRD;
	writeRegMask(FMC_SDCMR, ~0x003FFFFF, SDCMR);

	// 6. Set MODE bits to ‘011’, and configure the Target Bank bits
	//    (CTB1 and/or CTB2) as well as the number of consecutive Auto-refresh
	//    commands (NRFS) in the FMC_SDCMR register.
	//    Refer to the SDRAM datasheet for the number of Auto-refresh commands
	//    that should be issued. Typical number is 8.
	MODE	= 0b011 << 0;	// auto-refresh
	CTB1	= 0b1 << 4;		// CTB1 enable
	NRFS	= 8 << 5;		// typical number is 8, but according to following MT48LC4M32B manual it's at least 2 and must delay 100us between twice auto-refresh command.
	MRD		= 0 << 9;		// not use
	SDCMR	= MODE | CTB1 | NRFS | MRD;
	writeRegMask(FMC_SDCMR, ~0x003FFFFF, SDCMR);

	// 7. Configure the MRD field according to the SDRAM device, set the MODE bits
	//    to '100', and configure the Target Bank bits (CTB1 and/or CTB2) in
	//    the FMC_SDCMR register to issue a "Load Mode Register" command in order to
	//    program the SDRAM device. In particular:
	//    a) the CAS latency must be selected following configured value in
	//       FMC_SDCR1/2 registers
	//    b) the Burst Length (BL) of 1 must be selected by configuring the
	//       M[2:0] bits to 000 in the mode register. Refer to SDRAM device datasheet.
	//    If the Mode Register is not the same for both SDRAM banks,
	//    this step has to be repeated twice, once for each bank,
	//    and the Target Bank bits set accordingly.

	// The mode register of MT48LC4M32B can refer to following definition.
	//  -|---------------|----|---------|--------------|----|--------------|-
	//   | 12 | 11 | 10  |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
	//  -|---------------|----|---------|--------------|----|--------------|-
	//   |  Reserved     | WB | Op Mode | CAS Latency  | BT | Burst Length |
	//  -|---------------|----|---------|--------------|----|--------------|-
	uint16_t MODEREG_BURST_LENGTH	= 0b000 << 0;	// length = 1
	uint16_t MODEREG_BURST_TYPE		= 0b1 << 3;		// interleaved
	uint16_t MODEREG_CAS_LATENCY	= 0b011 << 4;	// latency = 3
	uint16_t MODEREG_OP_MODE		= 0b00 << 7;	// standard
	uint16_t MODEREG_WRITE_BURST	= 0b1 << 9;		// single
	uint16_t MODEREG = MODEREG_BURST_LENGTH | MODEREG_BURST_TYPE | MODEREG_CAS_LATENCY | MODEREG_OP_MODE | MODEREG_WRITE_BURST;
	MODE	= 0b100 << 0;	// load mode register
	CTB1	= 0b1 << 4;		// CTB1 enable
	NRFS	= 0 << 5;		// not use
	MRD		= MODEREG << 9;	// mode register
	SDCMR	= MODE | CTB1 | NRFS | MRD;
	writeRegMask(FMC_SDCMR, ~0x003FFFFF, SDCMR);

	// 8. Program the refresh rate in the FMC_SDRTR register
	//    The refresh rate corresponds to the delay between refresh cycles.
	//    Its value must be adapted to SDRAM devices.
	writeRegMaskThenWait(FMC_SDRTR, ~0x00003FFE, 1600 << 1);

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

// LTDC initialization
static void initLCD(void)
{
	// Initialize GPIO for LTDC
	initLCDGPIO();

	// Initialize DMA2D
	initDMA2D();

	// 1. Enable the LTDC clock in the RCC register.
	RCC->APB2ENR	|=	RCC_APB2ENR_LTDCEN;
	while((RCC->APB2ENR & RCC_APB2ENR_LTDCEN_Msk) == 0);

	// 2. Configure the required pixel clock following the panel datasheet.
	RCC->DCKCFGR1	&=	~RCC_DCKCFGR1_PLLSAIDIVR_Msk;
	RCC->DCKCFGR1	|=	0b10 << RCC_DCKCFGR1_PLLSAIDIVR_Pos; // PLLSAIDIVR = 8
	RCC->PLLSAICFGR = 	384 << RCC_PLLSAICFGR_PLLSAIN_Pos |
						5 << RCC_PLLSAICFGR_PLLSAIR_Pos |
						2 << RCC_PLLSAICFGR_PLLSAIQ_Pos | // not used
						0 << RCC_PLLSAICFGR_PLLSAIP_Pos;  // not used
	RCC->CR |= RCC_CR_PLLSAION;
	while((RCC->CR & RCC_CR_PLLSAIRDY_Msk) == 0);

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
	// while();

	// 4. Configure the synchronous signals and clock polarity in the LTDC_GCR register.
	// (*) Polarity assignment set by default value (low level valid).

	// 5. If needed, configure the background color in the LTDC_BCCR register.
	// (*) Background color set by default value (RGB888:0x000000).

	// 6. Configure the needed interrupts in the LTDC_IER and LTDC_LIPCR register.
	// (*) Temporarily donn't enable LTDC interrupt.

	// 7. Configure the layer1/2 parameters by:
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
	LTDC_Layer1->PFCR	=	0b001; // RGB888, AM480272H3TMQW-T01H LCD use RGB565

	//    – programming the color frame buffer start address in the LTDC_LxCFBAR register
	LTDC_Layer1->CFBAR	=	(uint32_t)&FrameBuffer; // Frame Buffer

	//    – programming the line length and pitch of the color frame buffer in the
	//      LTDC_LxCFBLR register
	LTDC_Layer1->CFBLR	=	(LCD_ACTIVE_WIDTH * LCD_COLOR_BYTES) << LTDC_LxCFBLR_CFBP_Pos |
							(LCD_ACTIVE_WIDTH * LCD_COLOR_BYTES + 3) << LTDC_LxCFBLR_CFBLL_Pos;
	// AM480272H3TMQW-T01H should multiply 2

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

	// 8. Enable layer1/2 and if needed the CLUT in the LTDC_LxCR register.
	LTDC_Layer1->CR |= LTDC_LxCR_LEN;

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
