/**********************************************************************
 * Copyright (c) 2018 - 2021 by Wang Yu
 * All rights reserved
 *
 * Filename:  stm32f746g_register.h
 * Project:   Minimum RTOS platform
 * Date:      2021/9/5
 * Author:    Wang Yu
 *
 **********************************************************************/
#ifndef STM32F746G_REGISTER_H
#define STM32F746G_REGISTER_H

// The following registers definition are refer to Cortex-M7
// System control registers
#define SCR_ACTLR			(0xE000E008UL)
#define SCR_SYST_CSR		(0xE000E010UL)
#define SCR_SYST_RVR		(0xE000E014UL)
#define SCR_SYST_CVR		(0xE000E018UL)
#define SCR_SYST_CALIB		(0xE000E01CUL)
#define SCR_CPUID			(0xE000ED00UL)
#define SCR_ICSR			(0xE000ED04UL)
#define SCR_VTOR			(0xE000ED08UL)
#define SCR_AIRCR			(0xE000ED0CUL)
#define SCR_SCR				(0xE000ED10UL)
#define SCR_CCR				(0xE000ED14UL)
#define SCR_SHPR1			(0xE000ED18UL)
#define SCR_SHPR2			(0xE000ED1CUL)
#define SCR_SHPR3			(0xE000ED20UL)
#define SCR_SHCSR			(0xE000ED24UL)
#define SCR_CFSR			(0xE000ED28UL)
#define SCR_HFSR			(0xE000ED2CUL)
#define SCR_DFSR			(0xE000ED30UL)
#define SCR_MMFAR			(0xE000ED34UL)
#define SCR_BFAR			(0xE000ED38UL)
#define SCR_ID_PFR0			(0xE000ED40UL)
#define SCR_ID_PFR1			(0xE000ED44UL)
#define SCR_CPACR			(0xE000ED88UL)
#define SYST_CSR			(0xE000E010UL)
#define SYST_RVR			(0xE000E014UL)
#define SYST_CVR			(0xE000E018UL)

// NVIC(Nested Vectored Interrupt Controller) registers
#define NVIC_ISER0			(0XE000E100UL)
#define NVIC_ISER1			(0XE000E104UL)
#define NVIC_ISER2			(0XE000E108UL)
#define NVIC_ISER3			(0XE000E10CUL)
#define NVIC_ISER4			(0XE000E110UL)
#define NVIC_ISER5			(0XE000E114UL)
#define NVIC_ISER6			(0XE000E118UL)
#define NVIC_ISER7			(0XE000E11CUL)
#define NVIC_ICER0			(0XE000E180UL)
#define NVIC_ICER1			(0XE000E184UL)
#define NVIC_ICER2			(0XE000E188UL)
#define NVIC_ICER3			(0XE000E18CUL)
#define NVIC_ICER4			(0XE000E190UL)
#define NVIC_ICER5			(0XE000E194UL)
#define NVIC_ICER6			(0XE000E198UL)
#define NVIC_ICER7			(0XE000E19CUL)
#define NVIC_IPR0			(0xE000E400UL)
#define NVIC_IPR1			(0xE000E404UL)
#define NVIC_IPR2			(0xE000E408UL)
#define NVIC_IPR3			(0xE000E40CUL)
#define NVIC_IPR4			(0xE000E410UL)
#define NVIC_IPR5			(0xE000E414UL)
#define NVIC_IPR6			(0xE000E418UL)
#define NVIC_IPR7			(0xE000E41CUL)
#define NVIC_IPR8			(0xE000E420UL)
#define NVIC_IPR9			(0xE000E424UL)
#define NVIC_IPR10			(0xE000E428UL)
#define NVIC_IPR11			(0xE000E42CUL)
#define NVIC_IPR12			(0xE000E430UL)
#define NVIC_IPR13			(0xE000E434UL)

// The following registers definition are refer to STM32F746G
#define _FLASH_AXIM_BASE	(0x08000000UL)
#define _PERIPH_BASE		(0x40000000UL)
#define _SDRAM_BANK1			(0xC0000000UL)

#define _APB1PERIPH_BASE	(_PERIPH_BASE + 0x00000000UL)
#define _APB2PERIPH_BASE	(_PERIPH_BASE + 0x00010000UL)
#define _AHB1PERIPH_BASE	(_PERIPH_BASE + 0x00020000UL)
#define _AHB2PERIPH_BASE	(_PERIPH_BASE + 0x10000000UL)
#define _AHB3PERIPH_BASE	(_PERIPH_BASE + 0x20000000UL) // External memory

#define _GPIOA_BASE			(_AHB1PERIPH_BASE + 0x0000UL)
#define _GPIOB_BASE			(_AHB1PERIPH_BASE + 0x0400UL)
#define _GPIOC_BASE			(_AHB1PERIPH_BASE + 0x0800UL)
#define _GPIOD_BASE			(_AHB1PERIPH_BASE + 0x0C00UL)
#define _GPIOE_BASE			(_AHB1PERIPH_BASE + 0x1000UL)
#define _GPIOF_BASE			(_AHB1PERIPH_BASE + 0x1400UL)
#define _GPIOG_BASE			(_AHB1PERIPH_BASE + 0x1800UL)
#define _GPIOH_BASE			(_AHB1PERIPH_BASE + 0x1C00UL)
#define _GPIOI_BASE			(_AHB1PERIPH_BASE + 0x2000UL)
#define _GPIOJ_BASE			(_AHB1PERIPH_BASE + 0x2400UL)
#define _GPIOK_BASE			(_AHB1PERIPH_BASE + 0x2800UL)
#define _RCC_BASE			(_AHB1PERIPH_BASE + 0x3800UL)
#define _FLASH_BASE			(_AHB1PERIPH_BASE + 0x3C00UL)
#define _FMC_BASE			(_AHB3PERIPH_BASE + 0x40000000UL)
#define _PWR_BASE			(_APB1PERIPH_BASE + 0x7000UL)
#define _TIM7_BASE			(_APB1PERIPH_BASE + 0x1400UL)
#define _USART1_BASE		(_APB2PERIPH_BASE + 0x1000UL)

// Flash register definition
#define FLASH_ACR			(_FLASH_BASE + 0x00UL)

// Power register definition
#define PWR_CR1				(_PWR_BASE + 0x00UL)
#define PWR_CSR1			(_PWR_BASE + 0x04UL)
#define PWR_CR2				(_PWR_BASE + 0x08UL)
#define PWR_CSR2			(_PWR_BASE + 0x0CUL)

// Timer7 register definition
#define TIM7_CR1			(_TIM7_BASE + 0x00UL)
#define TIM7_CR2			(_TIM7_BASE + 0x04UL)
#define TIM7_DIER			(_TIM7_BASE + 0x0CUL)
#define TIM7_SR				(_TIM7_BASE + 0x10UL)
#define TIM7_EGR			(_TIM7_BASE + 0x14UL)
#define TIM7_CNT			(_TIM7_BASE + 0x24UL)
#define TIM7_PSC			(_TIM7_BASE + 0x28UL)
#define TIM7_ARR			(_TIM7_BASE + 0x2CUL)

// RCC(Reset Clock Control) register definition
#define RCC_CR				(_RCC_BASE + 0x00UL)
#define RCC_PLLCFGR			(_RCC_BASE + 0x04UL)
#define RCC_CFGR			(_RCC_BASE + 0x08UL)
#define RCC_CIR				(_RCC_BASE + 0x0CUL)
#define RCC_AHB1RSTR		(_RCC_BASE + 0x10UL)
#define RCC_AHB2RSTR		(_RCC_BASE + 0x14UL)
#define RCC_AHB3RSTR		(_RCC_BASE + 0x18UL)
#define RCC_APB1RSTR		(_RCC_BASE + 0x20UL)
#define RCC_APB2RSTR		(_RCC_BASE + 0x24UL)
#define RCC_AHB1ENR			(_RCC_BASE + 0x30UL)
#define RCC_AHB2ENR			(_RCC_BASE + 0x34UL)
#define RCC_AHB3ENR			(_RCC_BASE + 0x38UL)
#define RCC_APB1ENR			(_RCC_BASE + 0x40UL)
#define RCC_APB2ENR			(_RCC_BASE + 0x44UL)
#define RCC_BDCR			(_RCC_BASE + 0x70UL)
#define RCC_CSR				(_RCC_BASE + 0x74UL)
#define RCC_SSCGR			(_RCC_BASE + 0x80UL)
#define RCC_PLLI2SC_FGR		(_RCC_BASE + 0x84UL)
#define RCC_PLLSAI_CFGR		(_RCC_BASE + 0x88UL)
#define RCC_DCKCF_GR1 		(_RCC_BASE + 0x8CUL)
#define RCC_DCKCF_GR2 		(_RCC_BASE + 0x90UL)

// FMC(Flexible Memory Control) register definition
#define FMC_SDCR1			(_FMC_BASE + 0x140UL)
#define FMC_SDCR2			(_FMC_BASE + 0x144UL)
#define FMC_SDTR1			(_FMC_BASE + 0x148UL)
#define FMC_SDTR2			(_FMC_BASE + 0x14CUL)
#define FMC_SDCMR			(_FMC_BASE + 0x150UL)
#define FMC_SDRTR			(_FMC_BASE + 0x154UL)
#define FMC_SDSR			(_FMC_BASE + 0x158UL)

// USART1 register definition
#define USART1_CR1			(_USART1_BASE + 0x00UL)
#define USART1_CR2			(_USART1_BASE + 0x04UL)
#define USART1_CR3			(_USART1_BASE + 0x08UL)
#define USART1_BRR			(_USART1_BASE + 0x0CUL)
#define USART1_GTPR			(_USART1_BASE + 0x10UL)
#define USART1_RTOR			(_USART1_BASE + 0x14UL)
#define USART1_RQR			(_USART1_BASE + 0x18UL)
#define USART1_ISR			(_USART1_BASE + 0x1CUL)
#define USART1_ICR			(_USART1_BASE + 0x20UL)
#define USART1_RDR			(_USART1_BASE + 0x24UL)
#define USART1_TDR			(_USART1_BASE + 0x28UL)

// GPIOA register definition
#define GPIOA_MODER			(_GPIOA_BASE + 0x00UL)
#define GPIOA_OTYPER		(_GPIOA_BASE + 0x04UL)
#define GPIOA_OSPEEDR		(_GPIOA_BASE + 0x08UL)
#define GPIOA_PUPDR			(_GPIOA_BASE + 0x0CUL)
#define GPIOA_IDR			(_GPIOA_BASE + 0x10UL)
#define GPIOA_ODR			(_GPIOA_BASE + 0x14UL)
#define GPIOA_BSRR			(_GPIOA_BASE + 0x18UL)
#define GPIOA_LCKR			(_GPIOA_BASE + 0x1CUL)
#define GPIOA_AFRL			(_GPIOA_BASE + 0x20UL)
#define GPIOA_AFRH			(_GPIOA_BASE + 0x24UL)
// GPIOB register definition
#define GPIOB_MODER			(_GPIOB_BASE + 0x00UL)
#define GPIOB_OTYPER		(_GPIOB_BASE + 0x04UL)
#define GPIOB_OSPEEDR		(_GPIOB_BASE + 0x08UL)
#define GPIOB_PUPDR			(_GPIOB_BASE + 0x0CUL)
#define GPIOB_IDR			(_GPIOB_BASE + 0x10UL)
#define GPIOB_ODR			(_GPIOB_BASE + 0x14UL)
#define GPIOB_BSRR			(_GPIOB_BASE + 0x18UL)
#define GPIOB_LCKR			(_GPIOB_BASE + 0x1CUL)
#define GPIOB_AFRL			(_GPIOB_BASE + 0x20UL)
#define GPIOB_AFRH			(_GPIOB_BASE + 0x24UL)

// GPIOI register definition
#define GPIOI_MODER			(_GPIOI_BASE + 0x00UL)
#define GPIOI_OTYPER		(_GPIOI_BASE + 0x04UL)
#define GPIOI_OSPEEDR		(_GPIOI_BASE + 0x08UL)
#define GPIOI_PUPDR			(_GPIOI_BASE + 0x0CUL)
#define GPIOI_IDR			(_GPIOI_BASE + 0x10UL)
#define GPIOI_ODR			(_GPIOI_BASE + 0x14UL)
#define GPIOI_BSRR			(_GPIOI_BASE + 0x18UL)
#define GPIOI_LCKR			(_GPIOI_BASE + 0x1CUL)
#define GPIOI_AFRL			(_GPIOI_BASE + 0x20UL)
#define GPIOI_AFRH			(_GPIOI_BASE + 0x24UL)

// GPIO register definition
#define GPIO_MODER(group)	(GPIO##group##_BASE + 0x00UL)
#define GPIO_OTYPER(group)	(GPIO##group##_BASE + 0x04UL)
#define GPIO_OSPEEDR(group)	(GPIO##group##_BASE + 0x08UL)
#define GPIO_PUPDR(group)	(GPIO##group##_BASE + 0x0CUL)
#define GPIO_IDR(group)		(GPIO##group##_BASE + 0x10UL)
#define GPIO_ODR(group)		(GPIO##group##_BASE + 0x14UL)
#define GPIO_BSRR(group)	(GPIO##group##_BASE + 0x18UL)
#define GPIO_LCKR(group)	(GPIO##group##_BASE + 0x1CUL)
#define GPIO_AFRL(group)	(GPIO##group##_BASE + 0x20UL)
#define GPIO_AFRH(group)	(GPIO##group##_BASE + 0x24UL)

#endif
