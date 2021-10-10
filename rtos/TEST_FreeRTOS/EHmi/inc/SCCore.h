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
#ifndef __SCL_CORE_H__
#define __SCL_CORE_H__

#include "SCBase.h"
#include "EHmiAlloc.h"

class SCCore : public EHmiAlloc {
public :
	SCCore(const SCRect& rect);
	SCCore(
		const unsigned short x,
		const unsigned short y,
		const unsigned short w,
		const unsigned short h
	);
	virtual ~SCCore() = 0;

public :
	SCRect	GetArea(void) const;

protected :
	SCRect	m_area;
};

#endif // __SCL_CORE_H__
