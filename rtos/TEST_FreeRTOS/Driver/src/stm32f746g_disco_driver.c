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
#include "stm32f746g_register.h"
#include "stm32f746g_disco_driver.h"

#define PLLM					((uint32_t)( 25 <<  0))
#define PLLN					((uint32_t)(432 <<  6))
#define PLLP					((uint32_t)(  0 << 16)) // pay attention to this value!!
#define PLLSRC					((uint32_t)(  1 << 22))
#define PLLQ					((uint32_t)(  9 << 24))

#define GPIO_MODER_IN			(0x0)
#define GPIO_MODER_OUT			(0x1)
#define GPIO_MODER_MULTI		(0x2)
#define GPIO_MODER_SIM			(0x3)
#define GPIO_OTYPER_PP			(0x0)
#define GPIO_OTYPER_OD			(0x1)
#define GPIO_OSPEEDR_LOW		(0x0)
#define GPIO_OSPEEDR_MID		(0x1)
#define GPIO_OSPEEDR_HIGH		(0x2)
#define GPIO_OSPEEDR_FULL		(0x3)
#define GPIO_PUPDR_NONE			(0x0)
#define GPIO_PUPDR_UP			(0x1)
#define GPIO_PUPDR_DOWN			(0x2)
#define GPIO_PUPDR_RESERVE		(0x3)
#define GPIO_AFR_AF7			(0x7)

typedef unsigned int			uint32_t;

static void initFPU(void);
static void initSystemClock(void);
static void initNVICPriorityGroup(void);
static void enableSystickInt(void);
static void enableUSART1Int(void);
static void initSystick(void);
static void initLED1(void);
static void initUSART1(void);
static void initTIM7(void);
static void initGPIOA9(void);
static void initGPIOB7(void);
static void initGPIOI1(void);

// Driver API
void initBoard(void)
{
	initFPU();
	initNVICPriorityGroup();
	initSystemClock();
	initLED1();
	initUSART1();
	initSystick();
	initTIM7();
}

void toggleLED1(void)
{
	uint32_t pin = 1;
	uint32_t temp, data;

	temp = *((uint32_t *)GPIOI_ODR);
	data = !((temp & (0x1 << pin)) >> pin);
	temp &= ~(0x1 << pin);
	temp |= (data << pin);
	*((uint32_t *)GPIOI_ODR) = temp;
}

void usart1SendChar(const char character)
{
	*((uint32_t *)USART1_CR1) |= 0x00000008; // enable TE
	*((uint32_t *)USART1_TDR) = (0x000000FF & character); // set character to TX buffer
	while((*((uint32_t *)USART1_ISR) & 0x00000040) == 0); // wait TC set
	*((uint32_t *)USART1_CR1) &= ~0x00000008; // disable TE
}

void usart1SendBuffer(const char* message)
{
	*((uint32_t *)USART1_CR1) |= 0x00000008; // enable TE
	for(uint32_t i = 0; i < strlen(message); i++) {
		*((uint32_t *)USART1_TDR) = (0x000000FF & message[i]); // set character to TX buffer
		while((*((uint32_t *)USART1_ISR) & 0x00000080) == 0); // wait TXE set
	}
	while((*((uint32_t *)USART1_ISR) & 0x00000040) == 0); // wait TC set
	*((uint32_t *)USART1_CR1) &= ~0x00000008; // disable TE
}

// The following initialization process is write for STM32F746G-DISCO
// FPU initialization
static void initFPU(void)
{
	// open CP10 & CP11
	*((uint32_t *)SCR_CPACR) |= (uint32_t)0x00F00000;
}

// System clock initialization
static void initSystemClock(void)
{
	// 1. Set HSE and reset RCC_CIR
	*((uint32_t *)RCC_CR) |= (uint32_t)0x00040000; // set HSEBYP bit
	*((uint32_t *)RCC_CR) |= (uint32_t)0x00010000; // set HSEON bit
	while((*((uint32_t *)RCC_CR) & 0x00020000) == 0); // wait HSERDY
	*((uint32_t *)RCC_CIR) = (uint32_t)0x00000000; // disable all RCC interrupts

	// 2. Set FLASH latency
	*((uint32_t *)FLASH_ACR) |= 0x00000007; // must be set in 216MHz
	while((*((uint32_t *)FLASH_ACR) & 0x0000000F) != 7); // wait latency set to 7
	// *((uint32_t *)FLASH_ACR) |= 0x00000300; // set ARTEN, PRFTEN
	//*((uint32_t *)PWR_CR1) |= 0x0000C000; // PWR_VOS default value has set to 0x11 in reset

	// 3. Enable PWR clock
	*((uint32_t *)RCC_APB1ENR) |= 0x10000000;

	// 4. Activation OverDrive Mode
	*((uint32_t *)PWR_CR1) |= 0x00010000; // set ODEN
	while((*((uint32_t *)PWR_CSR1) & 0x00010000) == 0); // wait ODRDY

	// 5. Activation OverDrive Switching
	*((uint32_t *)PWR_CR1) |= 0x00020000; // set ODSWEN
	while((*((uint32_t *)PWR_CSR1) & 0x00020000) == 0); // wait for ODSWRDY

	// 6. Main PLL configuration and activation
	*((uint32_t *)RCC_PLLCFGR) = PLLM | PLLN | PLLP | PLLSRC | PLLQ; // reset PLLCFGR register
	*((uint32_t *)RCC_CR) |= (uint32_t)0x01000000; // set PLLON
	while((*((uint32_t *)RCC_CR) & 0x02000000) == 0); // wait for PLLRDY

	// 7. System clock activation on the main PLL
	*((uint32_t *)RCC_CFGR) |= (uint32_t)0x00000000; // set HPRE  (AHB  pre-scaler) to 1 (216 MHz)
	*((uint32_t *)RCC_CFGR) |= (uint32_t)0x00001400; // set PPRE1 (APB1 pre-scaler) to 4 (54  MHz)
	*((uint32_t *)RCC_CFGR) |= (uint32_t)0x00008000; // set PPRE2 (APB2 pre-scaler) to 2 (108 MHz)
	while((*((uint32_t *)RCC_CFGR) & 0x0000FC00) != 0x00009400); // Before switching System clock to PLL, it must wait pre-scale value set to CFGR.
	*((uint32_t *)RCC_CFGR) |= (uint32_t)0x00000002; // set SW to PLL
	while((*((uint32_t *)RCC_CFGR) & 0x0000000C) != 0x00000008); // wait for system clock set to PLL
}

// set global NVIC priority group
static void initNVICPriorityGroup(void)
{
	// "|=" can not be used here, because VECTKEY write key is "0x05FA" and "|=" will change it
	*((uint32_t *)SCR_AIRCR) = (uint32_t)0x05FA0500; // PRIGROUP = 0b011 (main priority = 16, sub priority = 16)
	// global interrupt design
	// USART1(for Debug)	: main Group = 15, sub priority = 15
	// Systick				: main Group = 14, sub priority = 15
}

// enable USART1 interrupt
static void enableUSART1Int(void)
{
	// set USART1 interrupt priority
	*((uint32_t *)NVIC_IPR9) |= (uint32_t)(0xFF << 8); // main priority = 15, sub priority = 15, USART IRQn = 37

	// enable USART1 global interrupt
	*((uint32_t *)NVIC_ISER1) |= (uint32_t)0x00000020;
}

// enable Systick interrupt
static void enableSystickInt(void)
{
	// set Systick interrupt priority
	*((uint32_t *)SCR_SHPR3) |= (uint32_t)(0xAF << 24); // main priority = 10, sub priority = 15, Systick IRQn = 15

	// enable Systick global interrupt
	*((uint32_t *)SYST_CSR) |= (uint32_t)0x00000002; // enable Systick interrupt
}

// enable TIM7 interrupt
static void enableTIM7Int(void)
{
	// set TIM7 interrupt priority for LED1 flicker
	*((uint32_t *)NVIC_IPR13) |= (uint32_t)(0xEF << 24); // main priority = 14, sub priority = 15, TIM7 IRQn = 55

	// enable TIM7 global interrupt
	*((uint32_t *)NVIC_ISER1) |= (uint32_t)0x00800000;
	
	// enable TIM7 update interrupt
	*((uint32_t *)TIM7_DIER) |= (uint32_t)0x00000001;
}

// LED1 initialization
static void initLED1(void)
{
	initGPIOI1();
}

// USART1 initialization
static void initUSART1(void)
{
	// initialize USART1 TX/RX pin
	initGPIOA9(); // TX
	initGPIOB7(); // RX

	// enable APB2 USART1 RCC
	*((uint32_t *)RCC_APB2ENR) |= 0x00000010;
	while((*((uint32_t *)RCC_APB2ENR) & 0x00000010) == 0); // wait APB2 USART1 RCC set

	// set USART1 parameter
	//*((uint32_t *)USART1_BRR) |= 0x00002BF2; // baud rate = 9600(fCK=108MHz, CR1/OVER8=0, 0x2BF2 = 108000000/9600)
	*((uint32_t *)USART1_BRR) |= 0x000003AA; // baud rate = 115200(fCK=108MHz, CR1/OVER8=0, 0x03AA = 108000000/115200)
	*((uint32_t *)USART1_CR1) |= 0x00000000; // data bits = 8
	*((uint32_t *)USART1_CR2) |= 0x00000000; // stop bits = 1
	*((uint32_t *)USART1_CR1) |= 0x00000000; // parity = none (odd:0x00000600, even:0x00000400)
	*((uint32_t *)USART1_CR3) |= 0x00000000; // disable CTS and RTS

	// enable USART1 global interrupt
	enableUSART1Int();

	// enable RX interrupt
	*((uint32_t *)USART1_CR1) |= 0x00000020; // set RXNEIE

	// enable USART1
	*((uint32_t *)USART1_CR1) |= 0x00000001; // enable USART1

	// enable RE
	*((uint32_t *)USART1_CR1) |= 0x00000004; // enable RE
}

// TIM7 initialization for low speed timer(500ms)
static void initTIM7(void)
{
	// enable APB1 TIM7 RCC
	*((uint32_t *)RCC_APB1ENR) |= 0x00000020;
	while((*((uint32_t *)RCC_APB1ENR) & 0x00000020) == 0); // wait APB1 TIM7 RCC set
	
	// enable TIM7 interrupt
	enableTIM7Int();

	// enable pre-load ARPE
	*((uint32_t *)TIM7_CR1) |= (uint32_t)0x00000080;
	
	// set 4000 to pre-scale
	*((uint32_t *)TIM7_PSC) = (uint32_t)(0x00000FA0 - 1); // timer frequency = 108MHz / 4000 = 27KHz (APB pre-sacle is not 1, so fCK_PSC = 54MHz * 2 = 108MHz)
	while((*((uint32_t *)TIM7_PSC) & 0x0000FFFF) != (uint32_t)(0x00000FA0 - 1)); // wait PSC set
	
	// set 0 to CNT
	//*((uint32_t *)TIM7_CNT) = (uint32_t)0x00000000; // reset CNT

	// clear timer update interrupt flag
	*((uint32_t *)TIM7_SR) &= (uint32_t)~0x00000001; // reset UIF
	
	// set 13500 to reload value
	*((uint32_t *)TIM7_ARR) = (uint32_t)(0x000034BC - 1); // 500ms interrupt
	while((*((uint32_t *)TIM7_ARR) & 0x0000FFFF) != (uint32_t)(0x000034BC - 1)); // wait ARR set
	
	// enable TIM7 CEN
	*((uint32_t *)TIM7_CR1) |= (uint32_t)0x00000001;
}

// System tick initialization
static void initSystick(void)
{
	// enable Systick global interrupt
	enableSystickInt();

	// set RELOAD value (216000000 / 1000 = 216000)
	*((uint32_t *)SYST_RVR) = (uint32_t)(0x00034BC0 - 1); // 1ms interrupt
	while((*((uint32_t *)SYST_RVR) & 0x00FFFFFF) != (uint32_t)(0x00034BC0 - 1)); // wait for RELOAD value

	// set current value
	*((uint32_t *)SYST_CVR) = (uint32_t)0x00000000;

	// set CLKSOURCE, ENABLE
	*((uint32_t *)SYST_CSR) |= (uint32_t)0x00000005;
	while((*((uint32_t *)SYST_CSR) & 0x00000007) != (uint32_t)(0x00000007)); // wait for enable
}

// USART TX pin initialization
static void initGPIOA9(void)
{
	uint32_t pin  = 9;
	uint32_t temp = 0x00000000;

	// enable AHB1 GPIOA RCC
	*((uint32_t *)RCC_AHB1ENR) |= 0x00000001;
	while((*((uint32_t *)RCC_AHB1ENR) & 0x00000001) == 0); // wait AHB1 GPIOA RCC set

	// set GPIO MODER register
	temp = *((uint32_t *)GPIOA_MODER);
	temp &= ~(0x3 << (pin * 2)); // clear bits
	temp |= (GPIO_MODER_MULTI << (pin * 2)); // set bits
	*((uint32_t *)GPIOA_MODER) = temp;

	// set GPIO OTYPER register
	temp = *((uint32_t *)GPIOA_OTYPER);
	temp &= ~(0x1 << pin);
	temp |= (GPIO_OTYPER_PP << pin);
	*((uint32_t *)GPIOA_OTYPER) = temp;

	// set GPIO OSPEEDR register
	temp = *((uint32_t *)GPIOA_OSPEEDR);
	temp &= ~(0x3 << (pin * 2));
	temp |= (GPIO_OSPEEDR_FULL << (pin * 2));
	*((uint32_t *)GPIOA_OSPEEDR) = temp;

	// set GPIO PUPDR register
	temp = *((uint32_t *)GPIOA_PUPDR);
	temp &= ~(0x3 << (pin * 2));
	temp |= (GPIO_PUPDR_UP << (pin * 2));
	*((uint32_t *)GPIOA_PUPDR) = temp;

	// set GPIO AFRH register
	temp = *((uint32_t *)GPIOA_AFRH);
	temp &= ~(0x7 << ((pin - 8) * 4));
	temp |= (GPIO_AFR_AF7 << ((pin - 8) * 4));
	*((uint32_t *)GPIOA_AFRH) = temp;
}

// USART RX pin initialization
static void initGPIOB7(void)
{
	uint32_t pin  = 7;
	uint32_t temp = 0x00000000;

	// enable AHB1 GPIOB RCC
	*((uint32_t *)RCC_AHB1ENR) |= 0x00000002;
	while((*((uint32_t *)RCC_AHB1ENR) & 0x00000002) == 0); // wait AHB1 GPIOB RCC set

	// set GPIO MODER register
	temp = *((uint32_t *)GPIOB_MODER);
	temp &= ~(0x3 << (pin * 2)); // clear bits
	temp |= (GPIO_MODER_MULTI << (pin * 2)); // set bits
	*((uint32_t *)GPIOB_MODER) = temp;

	// set GPIO OTYPER register
	temp = *((uint32_t *)GPIOB_OTYPER);
	temp &= ~(0x1 << pin);
	temp |= (GPIO_OTYPER_PP << pin);
	*((uint32_t *)GPIOB_OTYPER) = temp;

	// set GPIO AFRL register
	temp = *((uint32_t *)GPIOB_AFRL);
	temp &= ~(0x7 << (pin * 4));
	temp |= (GPIO_AFR_AF7 << (pin * 4));
	*((uint32_t *)GPIOB_AFRL) = temp;

	// RX line need not to set OSPEEDR and PUPDR
}

// LED1 pin initialization
static void initGPIOI1(void)
{
	uint32_t pin  = 1;
	uint32_t temp = 0x00000000;

	// enable AHB1 GPIOI RCC
	*((uint32_t *)RCC_AHB1ENR) |= 0x00000100;
	while((*((uint32_t *)RCC_AHB1ENR) & 0x00000100) == 0); // wait AHB1 GPIOI RCC set

	// set GPIO MODER register
	temp = *((uint32_t *)GPIOI_MODER);
	temp &= ~(0x3 << (pin * 2)); // clear bits
	temp |= (GPIO_MODER_OUT << (pin * 2)); // set bits
	*((uint32_t *)GPIOI_MODER) = temp;

	// set GPIO OTYPER register
	temp = *((uint32_t *)GPIOI_OTYPER);
	temp &= ~(0x1 << pin);
	temp |= (GPIO_OTYPER_PP << pin);
	*((uint32_t *)GPIOI_OTYPER) = temp;

	// set GPIO OSPEEDR register
	temp = *((uint32_t *)GPIOI_OSPEEDR);
	temp &= ~(0x3 << (pin * 2));
	temp |= (GPIO_OSPEEDR_FULL << (pin * 2));
	*((uint32_t *)GPIOI_OSPEEDR) = temp;

	// set GPIO PUPDR register
	temp = *((uint32_t *)GPIOI_PUPDR);
	temp &= ~(0x3 << (pin * 2));
	temp |= (GPIO_PUPDR_UP << (pin * 2));
	*((uint32_t *)GPIOI_PUPDR) = temp;
}
