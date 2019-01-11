///
/// file		SCButton.cpp
/// brief		HMI button class file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2019/01/03
///
#include "SCButton.h"

/// function	SCButton
/// brief		constructor
SCButton::SCButton(const short id,
		const SCRect& area,
		const XColor& fore_color,
		const XColor& back_color,
		const std::string& font) :
SCLabel(id, area, fore_color, back_color, font)
{
	memset(m_label, 0x0000, sizeof(m_label));
}

/// function	~SCButton
/// brief		deconstructor
SCButton::~SCButton()
{
}

/// function	Draw
/// brief		draw button part
///
/// param		none
/// return		success of failed
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

/// function	DrawBackground
/// brief		draw button part's background
///
/// param		none
/// return		success of failed
bool SCButton::DrawBackground(void)
{
    return MakeRect(m_area, COLOR(Black), getButtonColor());
}

/// function	TDown
/// brief		the point when mouse is captured
///
/// param		point		the point when mouse is captured
/// return		none
void SCButton::TDown(const SCPoint& point)
{
    SetPushed(true);
	ReDraw();
}

/// function	TUp
/// brief		the point when mouse is released
///
/// param		point		the point when mouse is released
/// return		none
void SCButton::TUp(const SCPoint& point)
{
    if(GetPushed())
    {
        SetPushed(false);
        ReDraw();
        DoCallback(SCCallbackTypeTAP);
    }
}

/// function	TMove
/// brief		the point when mouse is moved
///
/// param		point		the point when mouse is moved
/// return		none
void SCButton::TMove(const SCPoint& point)
{
    bool next = m_area.Contains(point);
    if(GetPushed() != next)
    {
        SetPushed(next);
		ReDraw();
    }
}

/// function	getButtonColor
/// brief		get button color
///
/// param		none
/// return		XColor
XColor SCButton::getButtonColor(void)
{
    return (GetPushed()) ? (COLOR(DarkGray)) : (m_back_color);
}

/// function	drawUnderLine
/// brief		draw button bottom line
///
/// param		none
/// return		success of failed
bool SCButton::drawUnderLine(void)
{
	SCPoint begin(m_area.x, m_area.y+m_area.height-2);
	SCPoint end(m_area.x+m_area.width-1, m_area.y+m_area.height-2);
	
	return MakeLine(begin, end, m_fore_color);
}

/// function	drawRightLine
/// brief		draw button right line
///
/// param		none
/// return		success of failed
bool SCButton::drawRightLine(void)
{
	SCPoint begin(m_area.x+m_area.width-2, m_area.y);
	SCPoint end(m_area.x+m_area.width-2, m_area.y+m_area.height-1);
	
	return MakeLine(begin, end, m_fore_color);
}
