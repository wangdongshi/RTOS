/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename:  EHmiEvent.h
 * Project:   Minimum RTOS platform
 * Date:      2018/11/26
 * Author:    WangYu
 *
 **********************************************************************/
#ifndef __EHMI_EVENT_H__
#define __EHMI_EVENT_H__

#include "FreeRTOS.h"

typedef union _EHmiEventParam{
	unsigned long	lp;
	unsigned int    ip[2];
	unsigned short  sp[4];
	
	_EHmiEventParam() {}
	_EHmiEventParam(unsigned long ul) {lp = ul;}
} EHmiEventParam;

typedef enum {
    HMI_EV_NONE,
    HMI_EV_KEYDOWN, // for test
} EHmiEventType;

///
/// class : EHmiEvent
/// HMI event class
///
class EHmiEvent {
public :
    EHmiEvent();
    EHmiEvent(EHmiEventType ev);
    EHmiEvent(EHmiEventType ev, unsigned long lp);
	/*
    EHmiEventInfo(EHmiEventType ev, 
			unsigned int ip1,
			unsigned int ip2
			);
    EHmiEventInfo(EHmiEventType ev,
			unsigned short sp1,
			unsigned short sp2,
			unsigned short sp3,
			unsigned short sp4
			);
	*/
    ~EHmiEvent();
    EHmiEventType GetEvent() {return(type);}
    unsigned long GetULArg() {return(arg.lp);}
	void SetULArg(unsigned long ul) {arg.lp = ul;}
	
private :
    EHmiEventType	type;
    EHmiEventParam	arg;
};

#endif // __EHMI_EVENT_H__
