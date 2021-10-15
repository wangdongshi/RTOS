/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : platform.h
 * Project  : Minimum RTOS platform
 * Date     : 2021/9/5
 * Author   : WangYu
 *
 **********************************************************************/
#ifndef __PLATFORM_H__
#define __PLATFORM_H__

// This file include platform API definition of data type, utility, driver and OS resource.

#include "types.h"
#include "assert_param.h"
#include "debug.h"
#include "stm32f746g_disco.h"
#include "ft5336.h"
#include "ff.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "timers.h"

#endif // __PLATFORM_H__
