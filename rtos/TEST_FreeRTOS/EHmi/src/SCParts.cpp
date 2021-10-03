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
m_callback(0),
m_visible(true),
m_pushed(false),
m_fore_color(foreColor),
m_back_color(backColor)
{
}

SCParts::~SCParts()
{
	if(m_callback) {
		removeAllCallbacks();
	}
}

bool SCParts::DrawBackground(void)
{
	return PaintRect(m_area, m_back_color);
}

bool SCParts::ReDraw(void)
{
	bool res = true;

	res &= DrawBackground();
	res &= Draw();

	return res;
}

bool SCParts::Update(void)
{
	if(GetVisible()) {
		return ReDraw();
	} else {
		return true;
	}
}

void SCParts::TDown(const SCPoint& point)
{
	m_pushed = true;
}

void SCParts::TUp(const SCPoint& point)
{
	if(m_pushed) {
		m_pushed = false;
		DoCallback(SCCallbackTypeTAP);
	}
}

void SCParts::TMove(const SCPoint& point)
{
	bool next = m_area.Contains(point);

	if(m_pushed != next) {
		m_pushed = next;
	}
}

void SCParts::AddCallback(SCCallback* cb)
{
	if(cb != 0) {
		SCCallback*	last = m_callback;

		if(last == 0) {
			m_callback = cb;
		} else {
			while(last->Next() != 0) {
				last = last->Next();
			}
			last->Next(cb);
		}
	}
}

void SCParts::DoCallback(const int type)
{
	SCCallback*	cb = m_callback;

	while(cb) {
		if(cb->GetType() == type) cb->DoCallback();
		cb = cb->Next();
	}
}

void SCParts::removeAllCallbacks(void)
{
	SCCallback* next;
	SCCallback*	cb = m_callback;

	while(cb) {
		next = cb->Next();
		delete cb;

		cb = next;
	}

	m_callback = 0;
}
