///
/// file		SCParts.cpp
/// brief		HMI part class file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/12/20
///
#include "SCParts.h"
#include "SCDrawCommand.h"


/// function	SCParts
/// brief		constructor
SCParts::SCParts(const short id, const SCRect& area,
		const XColor fore_color, const XColor back_color) :
SCCore(area),
m_id(id),
m_parent(0),
m_next(0),
m_prev(0),
m_callback(0),
m_visible(true),
m_pushed(false),
m_fore_color(fore_color),
m_back_color(back_color)
{
}

/// function	~SCParts
/// brief		deconstructor
SCParts::~SCParts()
{
	if(m_callback) {
		removeAllCallbacks();
	}
}

/// function	DrawBackground
/// brief		draw part's background
///
/// param		none
/// return		success or failed
bool SCParts::DrawBackground(void)
{
	return PaintRect(m_area, m_back_color);
}

/// function	ReDraw
/// brief		redraw the part include background
///
/// param		none
/// return		success or failed
bool SCParts::ReDraw(void)
{
	bool res = true;
	
	res &= DrawBackground();
	res &= Draw();
	
	return res;
}

/// function	Update
/// brief		update part by cycle
///
/// param		none
/// return		success or failed
bool SCParts::Update(void)
{
	if(GetVisible()) {
		return ReDraw();
	} else {
		return true;
	}
}

/// function	TDown
/// brief		process when mouse click is catched
///
/// param		point	the position of the mouse pointer when it's catched
/// return		none
void SCParts::TDown(const SCPoint& point)
{
	m_pushed = true;
}

/// function	TUp
/// brief		process when mouse click is released
///
/// param		point	the position of the mouse pointer when it's released
/// return		none
void SCParts::TUp(const SCPoint& point)
{
	if(m_pushed) {
		m_pushed = false;
		DoCallback(SCCallbackTypeTAP);
	}
}

/// function	TMove
/// brief		process when mouse move on screen
///
/// param		point	the position of the mouse's movement
/// return		none
void SCParts::TMove(const SCPoint& point)
{
	bool next = m_area.Contains(point);

	if(m_pushed != next) {
		m_pushed = next;
	}
}

/// function	AddCallback
/// brief		register the specified type call back function
///
/// param		cb		point of call back function
/// return		none
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

/// function	DoCallback
/// brief		execute the specified type call back function
///
/// param		type	type of call back function
/// return		none
void SCParts::DoCallback(const int type)
{
	SCCallback*	cb = m_callback;
	
	while(cb) {
		if(cb->GetType() == type) cb->DoCallback();
		cb = cb->Next();
	}
}

/// function	removeAllCallbacks
/// brief		remove all of call back function
///
/// param		none
/// return		none
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
