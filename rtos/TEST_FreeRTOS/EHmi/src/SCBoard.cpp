/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename:  SCBoard.cpp
 * Project:   Minimum RTOS platform
 * Date:      2018/12/20
 * Author:    WangYu
 *
 **********************************************************************/
#include "SCParts.h"
#include "SCDrawCommand.h"

SCBoard::SCBoard(const SCRect& area, const short id) :
SCCore(area),
m_screen_id(id),
m_child(0),
m_captured(0),
//m_dialog(0),
//m_parent(false),
m_touch_point()
{
}

SCBoard::~SCBoard()
{
	DestroyAllParts();
}

void SCBoard::RegistParts(SCParts* target)
{
	SCParts* last;
	
	if(target) {
		target->Parent(this);
		last = GetLastChild();

		if(last) {
			last->Next(target);
			target->Prev(last);
		} else {
			m_child = target;
			target->Prev(0);
		}
		target->Next(0);
	}
}

void SCBoard::UnregistParts(SCParts* target)
{
	if(target) {
		SCParts* prev = target->Prev();
		SCParts* next = target->Next();
		
		if(prev == 0) {
			m_child = next;
			if(next) next->Prev(0);
		} else {
			prev->Next(next);
			if(next) next->Prev(prev);
			target->Prev(0);
		}
		target->Next(0);
	}
}

void SCBoard::DestroyParts(SCParts*	target)
{
    if(target) {
        UnregistParts(target);
        delete target;
    }
}

void SCBoard::DestroyAllParts(void)
{
	SCParts*	target;
	SCParts*	next;

	target = GetChild();
	while(target) {
		next = target->Next();
		delete target;
		target = next;
	}

	m_child = NULL;
	m_captured = NULL;
}

void SCBoard::Initialize(void)
{
	SCParts* target = GetChild();

	while(target) {
		target->Initialize();
		target = target->Next();
	}
}

bool SCBoard::Draw(void)
{
	bool res = DrawBackground();
	
	if(m_child) {
		SCParts* child = m_child;
		do {
			res &= child->ReDraw();
			child = child->Next();
		} while(child);
	}
	
	return res;
}

bool SCBoard::DrawBackground(void)
{
	bool res = true;

	res &= InitializeLCD();
	res &= PaintRect(m_area, SC_COLOR_LIGHT_GRAY);

	return res;
}

bool SCBoard::Update(void)
{
	bool res = true;
	
	SCParts* child = m_child;
	while(child) {
		res &= child->Update();
		child = child->Next();
	}
	
	return res;
}

void SCBoard::TDown(const SCPoint& point)
{
	SCParts* target = GetOperableChild(point);

	if(target) {
		m_captured = target;
		m_captured->TDown(point);
	}
}

void SCBoard::TUp(const SCPoint& point)
{
	if(m_captured) {
		m_touch_point = point;
		m_captured->TUp(point);
		m_captured = 0;
	}
}

void SCBoard::TMove(const SCPoint& point)
{
	if(m_captured) {
		m_captured->TMove(point);
	}
}

SCParts* SCBoard::GetChild(const unsigned short x, const unsigned short y) const
{
	SCPoint	point(x, y);

	return (GetChild(point));
}

SCParts* SCBoard::GetChild(const SCPoint& point) const
{
	SCRect   area;
	SCParts* target = GetLastChild();

	while(target) {
		area = target->GetArea();
		if(area.Contains(point)) {
			return(target);
		}
		target = target->Prev();
	}

	return(target);
}

SCParts* SCBoard::GetChild(const short id) const
{
	SCParts* target = GetChild();
	
	while(target) {
		if(id == target->GetID()) {
			return(target);
		}
		target = target->Next();
	}

	return(target);
}

SCParts* SCBoard::GetLastChild(void) const
{
	SCParts*	next;
	SCParts*	target = GetChild();
	
	while(target) {
		next = target->Next();
		if(next == 0) {
			return(target);
		}
		target = next;
	}
	
	return(target);
}

SCParts* SCBoard::GetOperableChild(const SCPoint& point) const
{
	SCParts* target = GetLastChild();

	while(target) {
		if(target->GetVisible()) {
			SCRect area = target->GetArea();
			if(area.Contains(point)) {
				//if(target->GetEnable() == false) {
				//	target = 0;
				//}
				break;
			}
		}
		target = target->Prev();
	}

	return(target);
}
