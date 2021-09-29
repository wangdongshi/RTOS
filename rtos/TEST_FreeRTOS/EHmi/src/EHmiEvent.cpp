/**********************************************************************
 * Copyright (c) 2018 - 2021 by Wang Yu
 * All rights reserved
 *
 * Filename:  EHmiEvent.cpp
 * Project:   Minimum RTOS platform
 * Date:      2018/11/26
 * Author:    Wang Yu
 *
 **********************************************************************/
#include "EHmiEvent.h"

//using namespace std;

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
