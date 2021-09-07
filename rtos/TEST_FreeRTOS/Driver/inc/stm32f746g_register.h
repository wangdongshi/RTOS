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
#define FLASH_AXIM_BASE		(0x08000000UL)
#define PERIPH_BASE			(0x40000000UL)

#define APB1PERIPH_BASE		(PERIPH_BASE + 0x00000000UL)
#define APB2PERIPH_BASE		(PERIPH_BASE + 0x00010000UL)
#define AHB1PERIPH_BASE		(PERIPH_BASE + 0x00020000UL)
#define AHB2PERIPH_BASE		(PERIPH_BASE + 0x10000000UL)
#define AHB3PERIPH_BASE		(PERIPH_BASE + 0x20000000UL) // External memory

#define GPIOA_BASE			(AHB1PERIPH_BASE + 0x0000UL)
#define GPIOB_BASE			(AHB1PERIPH_BASE + 0x0400UL)
#define GPIOC_BASE			(AHB1PERIPH_BASE + 0x0800UL)
#define GPIOD_BASE			(AHB1PERIPH_BASE + 0x0C00UL)
#define GPIOE_BASE			(AHB1PERIPH_BASE + 0x1000UL)
#define GPIOF_BASE			(AHB1PERIPH_BASE + 0x1400UL)
#define GPIOG_BASE			(AHB1PERIPH_BASE + 0x1800UL)
#define GPIOH_BASE			(AHB1PERIPH_BASE + 0x1C00UL)
#define GPIOI_BASE			(AHB1PERIPH_BASE + 0x2000UL)
#define GPIOJ_BASE			(AHB1PERIPH_BASE + 0x2400UL)
#define GPIOK_BASE			(AHB1PERIPH_BASE + 0x2800UL)
#define RCC_BASE			(AHB1PERIPH_BASE + 0x3800UL)
#define FLASH_BASE			(AHB1PERIPH_BASE + 0x3C00UL)
#define PWR_BASE			(APB1PERIPH_BASE + 0x7000UL)
#define TIM7_BASE			(APB1PERIPH_BASE + 0x1400UL)
#define USART1_BASE			(APB2PERIPH_BASE + 0x1000UL)

// Flash register definition
#define FLASH_ACR			(FLASH_BASE + 0x00UL)

// Power register definition
#define PWR_CR1				(PWR_BASE + 0x00UL)
#define PWR_CSR1			(PWR_BASE + 0x04UL)
#define PWR_CR2				(PWR_BASE + 0x08UL)
#define PWR_CSR2			(PWR_BASE + 0x0CUL)

// Timer7 register definition
#define TIM7_CR1			(TIM7_BASE + 0x00UL)
#define TIM7_CR2			(TIM7_BASE + 0x04UL)
#define TIM7_DIER			(TIM7_BASE + 0x0CUL)
#define TIM7_SR				(TIM7_BASE + 0x10UL)
#define TIM7_EGR			(TIM7_BASE + 0x14UL)
#define TIM7_CNT			(TIM7_BASE + 0x24UL)
#define TIM7_PSC			(TIM7_BASE + 0x28UL)
#define TIM7_ARR			(TIM7_BASE + 0x2CUL)

// RCC(Reset Clock Control) register definition
#define RCC_CR				(RCC_BASE + 0x00UL)
#define RCC_PLLCFGR			(RCC_BASE + 0x04UL)
#define RCC_CFGR			(RCC_BASE + 0x08UL)
#define RCC_CIR				(RCC_BASE + 0x0CUL)
#define RCC_AHB1RSTR		(RCC_BASE + 0x10UL)
#define RCC_AHB2RSTR		(RCC_BASE + 0x14UL)
#define RCC_AHB3RSTR		(RCC_BASE + 0x18UL)
#define RCC_APB1RSTR		(RCC_BASE + 0x20UL)
#define RCC_APB2RSTR		(RCC_BASE + 0x24UL)
#define RCC_AHB1ENR			(RCC_BASE + 0x30UL)
#define RCC_AHB2ENR			(RCC_BASE + 0x34UL)
#define RCC_AHB3ENR			(RCC_BASE + 0x38UL)
#define RCC_APB1ENR			(RCC_BASE + 0x40UL)
#define RCC_APB2ENR			(RCC_BASE + 0x44UL)
#define RCC_BDCR			(RCC_BASE + 0x70UL)
#define RCC_CSR				(RCC_BASE + 0x74UL)
#define RCC_SSCGR			(RCC_BASE + 0x80UL)
#define RCC_PLLI2SC_FGR		(RCC_BASE + 0x84UL)
#define RCC_PLLSAI_CFGR		(RCC_BASE + 0x88UL)
#define RCC_DCKCF_GR1 		(RCC_BASE + 0x8CUL)
#define RCC_DCKCF_GR2 		(RCC_BASE + 0x90UL)

// USART1 register definition
#define USART1_CR1			(USART1_BASE + 0x00UL)
#define USART1_CR2			(USART1_BASE + 0x04UL)
#define USART1_CR3			(USART1_BASE + 0x08UL)
#define USART1_BRR			(USART1_BASE + 0x0CUL)
#define USART1_GTPR			(USART1_BASE + 0x10UL)
#define USART1_RTOR			(USART1_BASE + 0x14UL)
#define USART1_RQR			(USART1_BASE + 0x18UL)
#define USART1_ISR			(USART1_BASE + 0x1CUL)
#define USART1_ICR			(USART1_BASE + 0x20UL)
#define USART1_RDR			(USART1_BASE + 0x24UL)
#define USART1_TDR			(USART1_BASE + 0x28UL)

// GPIOA register definition
#define GPIOA_MODER			(GPIOA_BASE + 0x00UL)
#define GPIOA_OTYPER		(GPIOA_BASE + 0x04UL)
#define GPIOA_OSPEEDR		(GPIOA_BASE + 0x08UL)
#define GPIOA_PUPDR			(GPIOA_BASE + 0x0CUL)
#define GPIOA_IDR			(GPIOA_BASE + 0x10UL)
#define GPIOA_ODR			(GPIOA_BASE + 0x14UL)
#define GPIOA_BSRR			(GPIOA_BASE + 0x18UL)
#define GPIOA_LCKR			(GPIOA_BASE + 0x1CUL)
#define GPIOA_AFRL			(GPIOA_BASE + 0x20UL)
#define GPIOA_AFRH			(GPIOA_BASE + 0x24UL)

// GPIOB register definition
#define GPIOB_MODER			(GPIOB_BASE + 0x00UL)
#define GPIOB_OTYPER		(GPIOB_BASE + 0x04UL)
#define GPIOB_OSPEEDR		(GPIOB_BASE + 0x08UL)
#define GPIOB_PUPDR			(GPIOB_BASE + 0x0CUL)
#define GPIOB_IDR			(GPIOB_BASE + 0x10UL)
#define GPIOB_ODR			(GPIOB_BASE + 0x14UL)
#define GPIOB_BSRR			(GPIOB_BASE + 0x18UL)
#define GPIOB_LCKR			(GPIOB_BASE + 0x1CUL)
#define GPIOB_AFRL			(GPIOB_BASE + 0x20UL)
#define GPIOB_AFRH			(GPIOB_BASE + 0x24UL)

// GPIOI register definition
#define GPIOI_MODER			(GPIOI_BASE + 0x00UL)
#define GPIOI_OTYPER		(GPIOI_BASE + 0x04UL)
#define GPIOI_OSPEEDR		(GPIOI_BASE + 0x08UL)
#define GPIOI_PUPDR			(GPIOI_BASE + 0x0CUL)
#define GPIOI_IDR			(GPIOI_BASE + 0x10UL)
#define GPIOI_ODR			(GPIOI_BASE + 0x14UL)
#define GPIOI_BSRR			(GPIOI_BASE + 0x18UL)
#define GPIOI_LCKR			(GPIOI_BASE + 0x1CUL)
#define GPIOI_AFRL			(GPIOI_BASE + 0x20UL)
#define GPIOI_AFRH			(GPIOI_BASE + 0x24UL)

#endif
