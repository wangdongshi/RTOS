/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : SCCore.cpp
 * Project  : Minimum RTOS platform
 * Date     : 2018/11/26
 * Author   : WangYu
 *
 **********************************************************************/
#include "SCCore.h"

SCCore::SCCore(const SCRect& rect) :
m_area(rect)
{
}

SCCore::SCCore(
	const unsigned short x,
	const unsigned short y,
	const unsigned short w,
	const unsigned short h
) : m_area(x, y, w, h)
{
}

SCCore::~SCCore()
{
}

SCRect SCCore::GetArea(void) const
{
	return m_area;
}

