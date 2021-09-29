///
/// file		EHmiEvent.cpp
/// brief		HMI event class file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/11/26
///
#include "EHmiEvent.h"

using namespace std;

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
