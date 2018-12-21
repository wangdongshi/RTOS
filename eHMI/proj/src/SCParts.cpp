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
//m_callback(0),
//m_fPushed(false),
m_fore_color(fore_color),
m_back_color(back_color)
{
}

/// function	DrawBackground
/// brief		draw part's background
///
/// param		none
/// return		success or failed
bool SCParts::DrawBackground(void)
{
	return PaintRect(m_area, SC_COLOR("LightGray"));
}
