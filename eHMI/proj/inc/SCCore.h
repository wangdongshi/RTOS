///
/// file		SCCore.h
/// brief		HMI parts base class define head file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/12/07
///

#ifndef __SCL_CORE_H__
#define __SCL_CORE_H__

#include "SCBase.h"

///
/// class : SCCore
/// Coordinate class
///
class SCCore {
public :
	SCCore(const SCRect& rect);
	SCCore(const unsigned short x, const unsigned short y,
			const unsigned short width, const unsigned short height);
	virtual ~SCCore() = 0;

	SCRect			GetArea(void) const;

protected :
	SCRect			m_area;
};

#endif // __SCL_CORE_H__
