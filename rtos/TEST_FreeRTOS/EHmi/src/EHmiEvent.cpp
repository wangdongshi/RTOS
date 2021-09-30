/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename:  EHmiEvent.cpp
 * Project:   Minimum RTOS platform
 * Date:      2018/11/26
 * Author:    WangYu
 *
 **********************************************************************/
#include "EHmiEvent.h"

EHmiEvent::EHmiEvent() : type(HMI_EV_NONE)
{
}

EHmiEvent::EHmiEvent(
	EHmiEventType ev
) : type(ev)
{
}

EHmiEvent::EHmiEvent(
	EHmiEventType ev,
	uint64_t ul
) : type(ev)
{
	arg.lp = ul;
}

EHmiEvent::EHmiEvent(
	EHmiEventType ev,
	uint32_t i1,
	uint32_t i2
) : type(ev)
{
	arg.ip[0] = i1;
	arg.ip[1] = i2;
}

EHmiEvent::EHmiEvent(
	EHmiEventType ev,
	uint16_t s1,
	uint16_t s2,
	uint16_t s3,
	uint16_t s4
) : type(ev)
{
	arg.sp[0] = s1;
	arg.sp[1] = s2;
	arg.sp[2] = s3;
	arg.sp[3] = s4;
}

EHmiEvent::~EHmiEvent()
{
}
