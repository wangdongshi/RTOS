///
/// file		EHmiEvent.cpp
/// brief		HMI event class file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/11/26
///
#include "debug.h"
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
/// param		ul : event param
/// return		none
EHmiEvent::EHmiEvent(EHmiEventType ev, unsigned long ul) :
	type(ev),
	arg(ul)
{
}

/// function	EHmiEvent
/// brief		constructor
///
/// param		ev : event type
/// param		i1 : event param1
/// param		i2 : event param2
/// return		none
EHmiEvent::EHmiEvent(EHmiEventType ev, unsigned int i1, unsigned int i2) :
type(ev),
arg(i1, i2)
{
}

/// function	~EHmiEvent
/// brief		deconstructor
EHmiEvent::~EHmiEvent()
{
}
