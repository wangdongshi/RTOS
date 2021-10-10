/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : EHmiEvent.h
 * Project  : Minimum RTOS platform
 * Date     : 2018/11/26
 * Author   : WangYu
 *
 **********************************************************************/
#ifndef __EHMI_EVENT_H__
#define __EHMI_EVENT_H__

#include "platform.h"

typedef union _EHmiEventParam{
	unsigned long	lp;
	unsigned int	ip[2];
	unsigned short	sp[4];
	
	_EHmiEventParam() {}
	_EHmiEventParam(unsigned long ul) {lp = ul;}
	_EHmiEventParam(unsigned int i1, unsigned int i2) {ip[0] = i1; ip[1] = i2;}
	_EHmiEventParam(unsigned short s1, unsigned short s2, unsigned short s3, unsigned short s4) {sp[0] = s1; sp[1] = s2; sp[2] = s3; sp[3] = s4;}
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
			unsigned long ul);
	EHmiEvent(EHmiEventType ev,
			unsigned int i1,
			unsigned int i2
			);
	EHmiEvent(EHmiEventType ev,
			unsigned short s1,
			unsigned short s2,
			unsigned short s3,
			unsigned short s4
			);
	~EHmiEvent();
	EHmiEventType GetEvent() {return(type);}
	unsigned long GetULong() {return(arg.lp);}
	unsigned int GetUInt(unsigned int arrayNo) {return(arg.ip[arrayNo]);}
	unsigned short GetUShort(unsigned short arrayNo) {return(arg.sp[arrayNo]);}
	void SetULong(unsigned long param) {arg.lp = param;}
	void SetUInt(unsigned int arrayNo, unsigned int param) {arg.ip[arrayNo] = param;}
	void SetUShort(unsigned int arrayNo, unsigned short param) {arg.sp[arrayNo] = param;}
	
private :
	EHmiEventType	type;
	EHmiEventParam	arg;
};

#endif // __EHMI_EVENT_H__
