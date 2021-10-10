/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : debug.c
 * Project  : Minimum RTOS platform
 * Date     : 2018/11/26
 * Author   : WangYu
 *
 **********************************************************************/
#include "debug.h"

#ifdef DEBUG

void TRACE(const char *format, ...)
{
	printf(format);
}

#endif
