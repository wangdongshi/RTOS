/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename:  debug.h
 * Project:   Minimum RTOS platform
 * Date:      2018/11/26
 * Author:    WangYu
 *
 **********************************************************************/
#ifndef DEBUG_H
#define DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#ifdef DEBUG

void TRACE(const char *format, ...);

#else

inline void TRACE(const char *format, ...)
{
}

#endif

#ifdef __cplusplus
}
#endif

#endif // DEBUG_H
