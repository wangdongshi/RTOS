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
#include <string.h>
#include "stm32f746g_disco_driver.h"

#define PLLM					((uint32_t)( 25 <<  0))
#define PLLN					((uint32_t)(432 <<  6))
#define PLLP					((uint32_t)(  0 << 16)) // pay attention to this value!!
#define PLLSRC					((uint32_t)(  1 << 22))
#define PLLQ					((uint32_t)(  9 << 24))

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

static uint32_t readRegister(uint32_t* addr, uint32_t shift, uint32_t len);
static void writeRegister(uint32_t* addr, uint32_t data, uint32_t shift, uint32_t len);
static void writeRegThenWait(uint32_t* addr, uint32_t data, uint32_t shift, uint32_t len);
static void writeRegMask(uint32_t* addr, uint32_t mask, uint32_t data);
static void writeRegMaskThenWait(uint32_t* addr, uint32_t mask, uint32_t data);
static void waitValueSet(uint32_t* addr, uint32_t mask, uint32_t data);
static void waitBitsSet(uint32_t* addr, uint32_t mask);
static void waitBitsClear(uint32_t* addr, uint32_t mask);

static void initFPU(void);
static void initSystemClock(void);
static void initNVICPriorityGroup(void);
static void initSystickInt(void);
static void initSVCallInt(void);
static void initPendSVInt(void);
static void initSystick(void);
static void initUSART1Int(void);
static void initLED1(void);
static void initUSART1(void);
static void initGPIOA9(void);
static void initGPIOB7(void);
static void initGPIOI1(void);

#ifdef MODE_STAND_ALONE
static void initTIM7Int(void);
static void initTIM7(void);
#endif


// Hardware driver API
void initBoard(void)
{
	initFPU();
	initNVICPriorityGroup();
	initSVCallInt();
	initPendSVInt();
	initSystemClock();
	initLED1();
	initUSART1();
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

// General register(4 bytes) operation function
static uint32_t readRegister(uint32_t* addr, uint32_t shift, uint32_t len)
{
	uint32_t* reg = addr;
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

static void writeRegister(uint32_t* addr, uint32_t data, uint32_t shift, uint32_t len)
{
	uint32_t* reg = addr;
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

static void writeRegThenWait(uint32_t* addr, uint32_t data, uint32_t shift, uint32_t len)
{
	uint32_t* reg = addr;
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

static void writeRegMask(uint32_t* addr, uint32_t mask, uint32_t data)
{
	volatile uint32_t* reg = addr;
	uint32_t val = *reg; 	// read from register

	// set data
	val &= mask;			// clear bits
	val |= data & (~mask);	// set bits
	*reg = val;				// write back to register
}

static void writeRegMaskThenWait(uint32_t* addr, uint32_t mask, uint32_t data)
{
	volatile uint32_t* reg = addr;
	uint32_t val = *reg; 	// read from register

	// set data
	val &= mask;			// clear bits
	val |= data & (~mask);	// set bits
	*reg = val;				// write back to register
	while(*reg != val);		// wait for set complete
}

static void waitValueSet(uint32_t* addr, uint32_t mask, uint32_t data)
{
	volatile uint32_t* reg = addr;
	while((*reg & ~mask) != data);

}

static void waitBitsSet(uint32_t* addr, uint32_t mask)
{
	volatile uint32_t* reg = addr;
	while((*reg & ~mask) == 0);
}

static void waitBitsClear(uint32_t* addr, uint32_t mask)
{
	volatile uint32_t* reg = addr;
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
	initGPIOI1();
}

// USART1 initialization
static void initUSART1(void)
{
	// Initialize USART1 TX/RX pin
	initGPIOA9(); // TX
	initGPIOB7(); // RX

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

// USART TX pin initialization
static void initGPIOA9(void)
{
	/*
	writeRegThenWait(RCC_AHB1ENR, 1, 0, 1);
	writeRegThenWait(GPIOA_MODER,	GPIO_MODER_MULTI,	9*2,		2);
	writeRegThenWait(GPIOA_OTYPER,	GPIO_OTYPER_PP,		9,			1);
	writeRegThenWait(GPIOA_OSPEEDR,	GPIO_OSPEEDR_FULL,	9*2,		2);
	writeRegThenWait(GPIOA_PUPDR,	GPIO_PUPDR_UP,		9*2,		2);
	writeRegThenWait(GPIOA_AFRH,	GPIO_AFR_AF7,		(9-8)*4,	4);
	*/
	writeRegMask(RCC_AHB1ENR,	~0x00000001,	0x00000001);
	writeRegMask(GPIOA_MODER,	~0x000C0000,	0x00080000);
	writeRegMask(GPIOA_OTYPER,	~0x00000200,	0x00000000);
	writeRegMask(GPIOA_OSPEEDR,	~0x000C0000,	0x000C0000);
	writeRegMask(GPIOA_PUPDR,	~0x000C0000,	0x00040000);
	writeRegMask(GPIOA_AFRH,	~0x000000F0,	0x00000070);
}

// USART RX pin initialization
static void initGPIOB7(void)
{
	/*
	writeRegThenWait(RCC_AHB1ENR, 1, 1, 1);
	writeRegThenWait(GPIOB_MODER,	GPIO_MODER_MULTI,	7*2,		2);
	writeRegThenWait(GPIOB_OTYPER,	GPIO_OTYPER_PP,		7,			1);
	writeRegThenWait(GPIOB_AFRL,	GPIO_AFR_AF7,		7*4,		4);
	*/
	writeRegMask(RCC_AHB1ENR,	~0x00000002,	0x00000002);
	writeRegMask(GPIOB_MODER,	~0x000C0000,	0x00080000);
	writeRegMask(GPIOB_OTYPER,	~0x00000200,	0x00000000);
	// RX line need not to set OSPEEDR and PUPDR
	writeRegMask(GPIOB_AFRL,	~0xF0000000,	0x70000000);
}

// LED1 pin initialization
static void initGPIOI1(void)
{
	writeRegThenWait(RCC_AHB1ENR,	0b1, 8, 1);
	writeRegThenWait(GPIOI_MODER,	GPIO_MODER_OUT,		1*2,		2);
	writeRegThenWait(GPIOI_OTYPER,	GPIO_OTYPER_PP,		1,			1);
	writeRegThenWait(GPIOI_OSPEEDR,	GPIO_OSPEEDR_FULL,	1*2,		2);
	writeRegThenWait(GPIOI_PUPDR,	GPIO_PUPDR_UP,		1*2,		2);
}
