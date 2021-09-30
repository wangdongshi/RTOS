/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename:  interrupt.h
 * Project:   Minimum RTOS platform
 * Date:      2021/9/5
 * Author:    WangYu
 *
 **********************************************************************/
#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "EHmiMain.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MODE_STAND_ALONE
extern char character;

// Timer7 500ms interrupt
void TIM7_IRQHandler(void);
void USART1_IRQHandler(void);
#endif

extern EHmiMain* pHmi;

void EXTI15_10_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif // __INTERRUPT_H__
