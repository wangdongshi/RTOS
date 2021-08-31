#include "stm32f746_register.h"

typedef unsigned int	uint32_t;

#define LED1_GPIO_SHIFT			(1) // PI1
#define DELAY_COUNT				(1000000)

#define PLLM					((uint32_t)( 16 <<  0))
#define PLLN					((uint32_t)(432 <<  6))
#define PLLP					((uint32_t)(  2 << 16))
#define PLLQ					((uint32_t)(  9 << 24))

#define GPIOI_MODER_IN			(0x0)
#define GPIOI_MODER_OUT			(0x1)
#define GPIOI_MODER_MULTI		(0x2)
#define GPIOI_MODER_SIM			(0x3)
#define GPIOI_OTYPER_PUSH_PULL	(0x0)
#define GPIOI_OTYPER_OPEN_DRAIN	(0x1)
#define GPIOI_OSPEEDR_LOW		(0x0)
#define GPIOI_OSPEEDR_MID		(0x1)
#define GPIOI_OSPEEDR_HIGH		(0x2)
#define GPIOI_OSPEEDR_FULL		(0x3)
#define GPIOI_PUPDR_NONE		(0x0)
#define GPIOI_PUPDR_PULL_UP		(0x1)
#define GPIOI_PUPDR_PULL_DOWN	(0x2)
#define GPIOI_PUPDR_RESERVE		(0x3)

void delay(unsigned int count);
void initSystemClock(void);
void initGPIOI(uint32_t pin);
void toggleLED1(void);

// main function for LED test
int main(void)
{
	// initialize board
	initSystemClock();
	initGPIOI(LED1_GPIO_SHIFT);

	// flick LED1 proclaim starting the demo
    for (uint32_t i = 0; i < 6; i++) {
    	toggleLED1();
		delay(DELAY_COUNT);
    }

    return 0;
}

// delay time is depend on system clock
void delay(unsigned int count)
{
	for(uint32_t i = 0; i < count; i++);
}

// The following initialization process is write for STM32F746G
void initSystemClock(void)
{
	// 0. Set HSE and reset RCC_CIR
	*((uint32_t *)RCC_CR) |= (uint32_t)0x00010000; // set HSEON  bit
	while((*((uint32_t *)RCC_CR) & 0x00000002) == 0); // Wait for HSERDY
	*((uint32_t *)RCC_CIR)  =  (uint32_t)0x00000000; // disable all RCC interrupts

	// 1. Set FLASH latency
	*((uint32_t *)FLASH_ACR) &= 0xFFFFFFF0;
	*((uint32_t *)FLASH_ACR) |= 0x00000007; // must in 216MHz
	//*((uint32_t *)PWR_CR1) |= 0x0000C000; // PWR_VOS default value has set to 0x11 in reset

	// 2. Enable PWR clock
	*((uint32_t *)RCC_APB1ENR) |= 0x10000000;

	// 3. Activation OverDrive Mode
	*((uint32_t *)PWR_CR1) |= 0x00010000; // set ODEN
	while((*((uint32_t *)PWR_CSR1) & 0x00010000) == 0); // Wait for ODRDY

	// 4. Activation OverDrive Switching
	*((uint32_t *)PWR_CR1) |= 0x00020000; // set ODSWEN
	while((*((uint32_t *)PWR_CSR1) & 0x00020000) == 0); // Wait for ODSWRDY

	// 5. Main PLL configuration and activation
	*((uint32_t *)RCC_PLLCFGR) = PLLM | PLLN | PLLP | PLLQ; // Reset PLLCFGR register
	*((uint32_t *)RCC_CR) |= (uint32_t)0x01000000; // set PLLON
	while((*((uint32_t *)RCC_CR) & 0x02000000) == 0); // Wait for PLLRDY

	// 6. System clock activation on the main PLL
	*((uint32_t *)RCC_CFGR) |= (uint32_t)0x00000000; // Set HPRE (AHB pre-scaler) to 1
	*((uint32_t *)RCC_CFGR) |= (uint32_t)0x00000002; // set SW to PLL
	while(((*((uint32_t *)RCC_CFGR) & 0x0000000C) >> 2) == 0x10); // Wait for system clock set to PLL

	// 7. Set APB1 & APB2 pre-scaler
	*((uint32_t *)RCC_CFGR) |= (uint32_t)0x00001400; // Set PPRE1 (APB1 pre-scaler) to 4
	*((uint32_t *)RCC_CFGR) |= (uint32_t)0x00008000; // Set PPRE2 (APB2 pre-scaler) to 2
}

void initGPIOI(uint32_t pin)
{
	uint32_t temp;

	// enable AHB1 GPIOI
	*((uint32_t *)RCC_AHB1ENR) |= 0x00000100;

	// set GPIO MODER register
	temp = *((uint32_t *)GPIOI_MODER_ADDR);
	temp &= ~(0x3 << (pin * 2)); // clear bits
	temp |= (GPIOI_MODER_OUT << (pin * 2)); // set bits
	*((uint32_t *)GPIOI_MODER_ADDR) = temp;

	// set GPIO OTYPER register
	temp = *((uint32_t *)GPIOI_OTYPER_ADDR);
	temp &= ~(0x1 << pin);
	temp |= (GPIOI_OTYPER_PUSH_PULL << pin);
	*((uint32_t *)GPIOI_OTYPER_ADDR) = temp;

	// set GPIO OSPEEDR register
	temp = *((uint32_t *)GPIOI_OSPEEDR_ADDR);
	temp &= ~(0x3 << (pin * 2));
	temp |= (GPIOI_OSPEEDR_FULL << (pin * 2));
	*((uint32_t *)GPIOI_OSPEEDR_ADDR) = temp;

	// set GPIO PUPDR register
	temp = *((uint32_t *)GPIOI_PUPDR_ADDR);
	temp &= ~(0x3 << (pin * 2));
	temp |= (GPIOI_PUPDR_PULL_UP << (pin * 2));
	*((uint32_t *)GPIOI_PUPDR_ADDR) = temp;
}

void toggleLED1(void)
{
	uint32_t temp, data;

	temp = *((uint32_t *)GPIOI_ODR_ADDR);
	data = !((temp & (0x1 << LED1_GPIO_SHIFT)) >> LED1_GPIO_SHIFT);
	temp &= ~(0x1 << LED1_GPIO_SHIFT);
	temp |= (data << LED1_GPIO_SHIFT);
	*((uint32_t *)GPIOI_ODR_ADDR) = temp;
}
