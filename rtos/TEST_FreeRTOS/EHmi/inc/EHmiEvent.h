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
	_EHmiEventParam(uint32_t i1, uint32_t i2) {ip[0] = i1; ip[1] = i2;}
	_EHmiEventParam(uint16_t s1, uint16_t s2, uint16_t s3, uint16_t s4) {sp[0] = s1; sp[1] = s2; sp[2] = s3; sp[3] = s4;}
} EHmiEventParam;

typedef enum {
    HMI_EV_NONE,
    HMI_EV_DATA_UPDATE,
    HMI_EV_CYCLIC_REFRESH, // 1s
    HMI_EV_TOUCH_DOWN,
    HMI_EV_TOUCH_UP,
    HMI_EV_TOUCH_MOVE,
    HMI_EV_SCREEN_CHG,
} EHmiEventType;

class EHmiEvent {
public :
    EHmiEvent();
    EHmiEvent(EHmiEventType ev);
    EHmiEvent(EHmiEventType ev,
    		uint64_t ul);
    EHmiEvent(EHmiEventType ev,
			uint32_t i1,
			uint32_t i2
			);
    EHmiEvent(EHmiEventType ev,
			uint16_t s1,
			uint16_t s2,
			uint16_t s3,
			uint16_t s4
			);
    ~EHmiEvent();
    EHmiEventType GetEvent() {return(type);}
    uint64_t GetULong() {return(arg.lp);}
    uint32_t GetUInt(uint32_t arrayNo) {return(arg.ip[arrayNo]);}
    uint16_t GetUShort(uint16_t arrayNo) {return(arg.sp[arrayNo]);}
	void SetULong(uint64_t param) {arg.lp = param;}
	void SetUInt(uint32_t arrayNo, uint32_t param) {arg.ip[arrayNo] = param;}
	void SetUShort(uint32_t arrayNo, uint16_t param) {arg.sp[arrayNo] = param;}
	
private :
    EHmiEventType	type;
    EHmiEventParam	arg;
};

#endif // __EHMI_EVENT_H__
