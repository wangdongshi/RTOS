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
static __attribute__((unused)) void waitBitsClear(uint32_t* addr, uint32_t mask);

static void initFPU(void);
static void initSDRAM(void);
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

#ifdef MODE_STAND_ALONE
static void initTIM7Int(void);
static void initTIM7(void);
#endif


// Hardware driver API
void initBoard(void)
{
	initFPU();
	initSystemClock();
	initSDRAM();
	initNVICPriorityGroup();
	initSVCallInt();
	initPendSVInt();
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
	NRFS	= 8 << 5;		// typical number is 8.
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
	uint16_t MODEREG_BURST_TYPE		= 0b0 << 3;		// sequential
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
}
