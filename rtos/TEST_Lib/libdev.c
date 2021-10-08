#include "libdev.h"
#include "stm32f746xx.h"

void toggle_LED1_from_lib(void)
{
	uint32_t data = GPIOI->ODR & GPIO_ODR_OD1_Msk;
	GPIOI->ODR &= ~GPIO_ODR_OD1_Msk;
	GPIOI->ODR |= (~data) & GPIO_ODR_OD1_Msk;
}
