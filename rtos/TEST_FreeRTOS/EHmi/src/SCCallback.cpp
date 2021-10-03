/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename:  SCCallback.cpp
 * Project:   Minimum RTOS platform
 * Date:	  2018/11/26
 * Author:	  WangYu
 *
 **********************************************************************/
#include "SCCallback.h"

SCCallback::SCCallback(const int type, void	(*func)(int), const int	data) :
m_func(func),
m_type(type),
m_data(data),
m_next(0)
{
}

SCCallback::~SCCallback(
)
{
}

void SCCallback::DoCallback(void)
{
	if (m_func) m_func(m_data);
}
