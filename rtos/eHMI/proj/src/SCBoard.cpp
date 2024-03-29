///
/// file		SCBoard.cpp
/// brief		HMI board class file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/12/20
///
#include "SCParts.h"
#include "SCDrawCommand.h"

/// function	SCBoard
/// brief		constructor
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

/// function	~SCBoard
/// brief		deconstructor
SCBoard::~SCBoard()
{
	DestroyAllParts();
}

/// function	RegistParts
/// brief		regist part to board
///
/// param		target		target part which will be registed
/// return		none
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

/// function	UnregistParts
/// brief		unregist part from board
///
/// param		target		target part which will be unregisted
/// return		none
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

/// function	DestroyParts
/// brief		destroy part from board
///
/// param		target		target part which will be destroied
/// return		none
void SCBoard::DestroyParts(SCParts*	target)
{
    if(target) {
        UnregistParts(target);
        delete target;
    }
}

/// function	DestroyAllParts
/// brief		destroy all part from board
///
/// param		none
/// return		none
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

/// function	Initialize
/// brief		initialize all parts on this board
///
/// param		none
/// return		none
void SCBoard::Initialize(void)
{
	SCParts* target = GetChild();

	while(target) {
		target->Initialize();
		target = target->Next();
	}
}

/// function	Draw
/// brief		draw background and all parts on this board
///
/// param		none
/// return		success or failed 
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

/// function	DrawBackground
/// brief		draw part's background
///
/// param		none
/// return		success or failed
bool SCBoard::DrawBackground(void)
{
	return PaintRect(m_area, COLOR(LightGray));
}

/// function	Update
/// brief		update screen by cycle
///
/// param		none
/// return		success or failed
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

/// function	TDown
/// brief		process when mouse click is caught on screen
///
/// param		point	the position of the mouse pointer when it's caught
/// return		none
void SCBoard::TDown(const SCPoint& point)
{
	SCParts* target = GetOperableChild(point);

	if(target) {
		m_captured = target;
		m_captured->TDown(point);
	}
}

/// function	TUp
/// brief		process when mouse click is released on screen
///
/// param		point	the position of the mouse pointer when it's released
/// return		none
void SCBoard::TUp(const SCPoint& point)
{
	if(m_captured) {
		m_touch_point = point;
		m_captured->TUp(point);
		m_captured = 0;
	}
}

/// function	TMove
/// brief		process when mouse move on screen
///
/// param		point	the position of the mouse's movement
/// return		none
void SCBoard::TMove(const SCPoint& point)
{
	if(m_captured) {
		m_captured->TMove(point);
	}
}

/// function	GetChild
/// brief		get child parts by it's coordinate
///
/// param		x		x coordinate
/// param		y		y coordinate
/// return		parts with the coordinate
SCParts* SCBoard::GetChild(const unsigned short x, const unsigned short y) const
{
	SCPoint	point(x, y);

	return (GetChild(point));
}

/// function	GetChild
/// brief		get child parts by it's coordinate
///
/// param		point	point's coordinate
/// return		parts with the coordinate
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

/// function	GetChild
/// brief		get child parts by it's ID
///
/// param		id		part's ID
/// return		parts with this ID
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

/// function	GetLastChild
/// brief		get last child parts
///
/// param		none
/// return		last parts
SCParts* SCBoard::GetLastChild(void) const
{
	SCParts* next;
	SCParts* target = GetChild();
	
	while(target) {
		next = target->Next();
		if(next == 0) {
			return(target);
		}
		target = next;
	}
	
	return(target);
}

/// function	GetOperableChild
/// brief		get operable child part on the screen
///
/// param		point	the specified point
/// return		part's pointer
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
