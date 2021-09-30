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

#include "platform.h"

typedef union _EHmiEventParam{
	uint64_t	lp;
	uint32_t	ip[2];
	uint16_t	sp[4];
	
	_EHmiEventParam() {}
	_EHmiEventParam(uint64_t ul) {lp = ul;}
} EHmiEventParam;

typedef enum {
    HMI_EV_NONE,
    HMI_EV_KEYDOWN, // for test
} EHmiEventType;

class EHmiEvent {
public :
    EHmiEvent();
    EHmiEvent(EHmiEventType ev);
    EHmiEvent(EHmiEventType ev, uint64_t lp);
#if 0
    EHmiEventInfo(EHmiEventType ev, 
			uint32_t ip1,
			uint32_t ip2
			);
    EHmiEventInfo(EHmiEventType ev,
			uint16_t sp1,
			uint16_t sp2,
			uint16_t sp3,
			uint16_t sp4
			);
#endif
    ~EHmiEvent();
    EHmiEventType GetEvent() {return(type);}
    uint64_t GetULArg() {return(arg.lp);}
	void SetULArg(uint64_t ul) {arg.lp = ul;}
	
private :
    EHmiEventType	type;
    EHmiEventParam	arg;
};

#endif // __EHMI_EVENT_H__
