/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : SCButton.cpp
 * Project  : Minimum RTOS platform
 * Date     : 2018/11/26
 * Author   : WangYu
 *
 **********************************************************************/
#include <string.h>
#include "SCButton.h"

SCButton::SCButton(const short id,
		const SCRect& area,
		const unsigned int fore_color,
		const unsigned int back_color,
		const unsigned int font) :
SCLabel(id, area, fore_color, back_color, font)
{
	memset(m_label, 0x0000, sizeof(m_label));
}

SCButton::~SCButton()
{
}

bool SCButton::Draw(void)
{
	bool res = true;
	
	if(GetVisible())
	{
		res &= drawUnderLine();
		res &= drawRightLine();
		res &= drawString();
	}
	
	return res;
}

bool SCButton::DrawBackground(void)
{
	return MakeRect(m_area, SC_COLOR_BLACK, getButtonColor());
}

void SCButton::TDown(const SCPoint& point)
{
	SetPushed(true);
	ReDraw();
}

void SCButton::TUp(const SCPoint& point)
{
	if(GetPushed())
	{
		SetPushed(false);
		ReDraw();
		DoCallback(SCCallbackTypeTAP);
	}
}

void SCButton::TMove(const SCPoint& point)
{
	bool next = m_area.Contains(point);
	if(GetPushed() != next)
	{
		SetPushed(next);
		ReDraw();
	}
}

unsigned int SCButton::getButtonColor(void)
{
	return (GetPushed()) ? SC_COLOR_DARK_GRAY : m_back_color;
}

bool SCButton::drawUnderLine(void)
{
	SCPoint begin(m_area.x, m_area.y + m_area.height - 2);
	SCPoint end(m_area.x + m_area.width - 1, m_area.y + m_area.height - 2);
	
	return MakeLine(begin, end, m_fore_color);
}

bool SCButton::drawRightLine(void)
{
	SCPoint begin(m_area.x + m_area.width - 2, m_area.y);
	SCPoint end(m_area.x + m_area.width - 2, m_area.y + m_area.height - 1);
	
	return MakeLine(begin, end, m_fore_color);
}
