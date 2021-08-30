#include "stm32f746_register.h"

typedef unsigned int	uint32_t;

#define LED1_GPIO_SHIFT	(1) // PI1
#define DELAY_COUNT		(100000)

void delay(unsigned int count);
void initSystemClock(void);
void initGPIOI(uint32_t pin);
void setLED1(void);
void resetLED1(void);
void toggleLED1(void);

// main function for LED test
int main(void)
{
	initSystemClock();
	initGPIOI(LED1_GPIO_SHIFT);

    while (1) {
    	toggleLED1();
		delay(DELAY_COUNT);
    }

    return 0;
}

// delay time is depend on system clock
void delay(unsigned int count)
{
	for (int i = 0; i < count; i++);
}

// The following initialization process is write for STM32F746G
void initSystemClock(void)
{
	*((uint32_t *)RCC_CR) |= (uint32_t)0x00090000; // Set HSEON, CSSON bit
	while((*((uint32_t *)RCC_CR) & 0x00000002) == 0); // Wait for HSERDY
	*((uint32_t *)RCC_CFGR)		=  (uint32_t)0x00000000; // Reset CFGR register
	*((uint32_t *)RCC_CR)		&= (uint32_t)0xFEF6FFFF; // Reset HSEON, CSSON and PLLON bits
	*((uint32_t *)RCC_PLLCFGR)	=  (uint32_t)0x00000000; // Reset PLLCFGR register
	*((uint32_t *)RCC_CIR)		=  (uint32_t)0x00000000; // Disable all interrupts
}

void initGPIOI(uint32_t pin)
{
	uint32_t  temp;

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

void setLED1(void)
{
	uint32_t  temp;

	temp = *((uint32_t *)GPIOI_ODR_ADDR);
	temp &= ~(0x1 << LED1_GPIO_SHIFT);
	temp |= (0x1 << LED1_GPIO_SHIFT);
	*((uint32_t *)GPIOI_ODR_ADDR) = temp;
}

void resetLED1(void)
{
	uint32_t  temp;

	temp = *((uint32_t *)GPIOI_ODR_ADDR);
	temp &= ~(0x1 << LED1_GPIO_SHIFT);
	temp |= (0x0 << LED1_GPIO_SHIFT);
	*((uint32_t *)GPIOI_ODR_ADDR) = temp;
}

void toggleLED1(void)
{
	uint32_t  temp;
	uint32_t  data;

	temp = *((uint32_t *)GPIOI_ODR_ADDR);
	data = !((temp & (0x1 << LED1_GPIO_SHIFT)) >> LED1_GPIO_SHIFT);
	temp &= ~(0x1 << LED1_GPIO_SHIFT);
	temp |= (data << LED1_GPIO_SHIFT);
	*((uint32_t *)GPIOI_ODR_ADDR) = temp;
}
