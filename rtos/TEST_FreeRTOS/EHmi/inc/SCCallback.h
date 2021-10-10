/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : SCCallback.h
 * Project  : Minimum RTOS platform
 * Date     : 2018/11/26
 * Author   : WangYu
 *
 **********************************************************************/
#ifndef __SCL_CALLBACK_H__
#define __SCL_CALLBACK_H__

#include "EHmiMain.h"

class EHmiMain;

#ifdef __cplusplus
extern "C" {
#endif

extern EHmiMain* pHmi;

#ifdef __cplusplus
}
#endif

// call back type
enum SCCallbackType {
	SCCallbackTypeTAP = 0,		// tap from touch panel
	SCCallbackTypeFLICK,		// flick from touch panel
	SCCallbackTypeLONGTAP,		// long tap from touch panel
	SCCallbackTypeCHANGEVALUE	// value change(if not to be used, delete it)
};

class SCCallback {
public :
	SCCallback(const int type, void (*func)(int), const int data);
	virtual ~SCCallback();

	void			DoCallback(void);

	SCCallback*		Next(void) const {return(m_next);}
	void			Next(SCCallback* next) {m_next = next;}

	int				GetType(void) const {return(m_type);}
	
private :
	void			(*m_func)(const int data);
	int				m_type;		// call back type
	int				m_data;		// parameter in call back function
	SCCallback*		m_next;		// next call back function point by registered sequence
};

#endif // __SCL_CALLBACK_H__

