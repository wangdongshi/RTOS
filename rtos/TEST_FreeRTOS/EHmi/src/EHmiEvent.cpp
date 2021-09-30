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

EHmiEvent::EHmiEvent(EHmiEventType ev) : type(ev)
{
}

EHmiEvent::EHmiEvent(EHmiEventType ev, uint64_t ul) :
	type(ev),
	arg(ul)
{
}

EHmiEvent::~EHmiEvent()
{
}
