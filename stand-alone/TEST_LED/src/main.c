#include "stm32f746_register.h"

typedef unsigned int	uint32_t;

#define LED1_GPIO_SHIFT	(1)			// PI1
#define DELAY_COUNT		(10000000)	// 5s

// delay time is depend on system clock
void delay(unsigned int count)
{
	for (volatile int i = 0; i < count; i++);
}

void setRegister(uint32_t* addr, uint32_t data, uint32_t shift, uint32_t len)
{
	volatile uint32_t* reg = addr;
	volatile uint32_t  val = *reg; // read from register
	uint32_t mask = 0x00000000;

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

void setRegMask(uint32_t* addr, uint32_t mask, uint32_t data)
{
	volatile uint32_t* reg = addr;
	volatile uint32_t  val = *reg; // read from register

	// set data
	val &= mask;			// clear bits
	val |= data & ~mask;	// set bits
	*reg = val;				// write back to register
	while(*reg != val);		// wait for set complete
}

// The following initialization process is write for STM32F746G
void initSystemClock(void)
{
	// Reset HSEON, CSSON and PLLON bits with default value
	setRegister(RCC_CR, 1, 0, 1);					// Set HSION bit
	setRegister(RCC_CFGR, 0x00000000, 0, 32);		// Reset CFGR register
	setRegister(RCC_PLLCFGR, 0x00000000, 0, 32);	// Reset PLLCFGR register
	setRegister(RCC_CIR, 0x00000000, 0, 32);		// Disable all interrupts
}

void initGPIOI(uint32_t pin)
{
	setRegister(RCC_AHB1ENR, 1, 8, 1);

	setRegister(GPIOI_MODER, GPIOI_MODER_OUT, pin*2, 2);
	setRegister(GPIOI_OTYPER, GPIOI_OTYPER_PUSH_PULL, pin, 1);
	setRegister(GPIOI_OSPEEDR, GPIOI_OSPEEDR_FULL, pin*2, 2);
	setRegister(GPIOI_PUPDR, GPIOI_PUPDR_PULL_UP, pin*2, 2);
}

// main function for LED test
int main(void)
{
	initSystemClock();
	initGPIOI(LED1_GPIO_SHIFT);

    while (1) {
		setRegister(GPIOI_ODR, 1, LED1_GPIO_SHIFT, 1);
		delay(DELAY_COUNT);
		setRegister(GPIOI_ODR, 0, LED1_GPIO_SHIFT, 1);
		delay(DELAY_COUNT);
    }

    return 0;
}
