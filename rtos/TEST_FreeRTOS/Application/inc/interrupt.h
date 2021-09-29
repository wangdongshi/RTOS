/**********************************************************************
 * Copyright (c) 2018 - 2021 by Wang Yu
 * All rights reserved
 *
 * Filename:  interrupt.h
 * Project:   Minimum RTOS platform
 * Date:      2021/9/5
 * Author:    Wang Yu
 *
 **********************************************************************/
#ifndef INTERRUPT_H
#define INTERRUPT_H

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

#endif // INTERRUPT_H
