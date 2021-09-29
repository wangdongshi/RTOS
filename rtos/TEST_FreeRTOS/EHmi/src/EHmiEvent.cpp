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

/// function	EHmiEvent
/// brief		constructor
///
/// param		none
/// return		none
EHmiEvent::EHmiEvent() : type(HMI_EV_NONE)
{
}

/// function	EHmiEvent
/// brief		constructor
///
/// param		ev : event type
/// return		none
EHmiEvent::EHmiEvent(EHmiEventType ev) : type(ev)
{
}

/// function	EHmiEvent
/// brief		constructor
///
/// param		ev : event type
/// param		ul : event type
/// return		none
EHmiEvent::EHmiEvent(EHmiEventType ev, unsigned long ul) :
	type(ev),
	arg(ul)
{
}

/// function	~EHmiEvent
/// brief		deconstructor
EHmiEvent::~EHmiEvent()
{
}
