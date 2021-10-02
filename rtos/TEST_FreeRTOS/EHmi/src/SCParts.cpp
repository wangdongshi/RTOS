/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename:  SCParts.cpp
 * Project:   Minimum RTOS platform
 * Date:      2018/12/20
 * Author:    WangYu
 *
 **********************************************************************/
#include "SCParts.h"
#include "SCDrawCommand.h"

SCParts::SCParts(const short id, const SCRect& area,
		const uint32_t foreColor, const uint32_t backColor) :
SCCore(area),
m_id(id),
m_parent(0),
m_next(0),
m_prev(0),
//m_callback(0),
//m_fPushed(false),
m_fore_color(foreColor),
m_back_color(backColor)
{
}

SCParts::~SCParts()
{
}

bool SCParts::DrawBackground(void)
{
	return PaintRect(m_area, m_back_color);
}

