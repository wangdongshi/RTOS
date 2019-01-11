///
/// file		SCDrawCommand.cpp
/// brief		HMI draw command class source file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/12/19
///
#include <math.h>
#include <string>
#include "debug.h"
#include "SCDrawCommand.h"

using namespace std;

/// function	SCDrawCommand
/// brief		constructor
SCDrawCommand::SCDrawCommand()
{
}

/// function	~SCDrawContext
/// brief		deconstructor
SCDrawCommand::~SCDrawCommand()
{
}

/// function	GetSysTime
/// brief		get current time
///
/// param		none
/// return		string's width (by pixel)
time_t SCDrawCommand::GetSysTime()
{
	time_t sys_time;
	
    time(&sys_time);
    sys_time += 8*3600;  // transform the time zone
	
    return sys_time;
}

/// function	GetStringWidth
/// brief		get width of string
///
/// param		str		target string
/// return		string's width (by pixel)
int SCDrawCommand::GetStringWidth(const unsigned short* str)
{
	int width = 0;

	while (*str++) {
		width += 7;
	}

	return width;
}

/// function	DrawPoint
/// brief		draw a point
///
/// param		x		start x coordinate
/// param		y		start y coordinate
/// param		color	rectangle's color
/// return		success or failed
bool SCDrawCommand::DrawPoint(
		const unsigned int x,
		const unsigned int y,
		const XColor& color)
{
	return drawRect(x, y, 1, 1, color);
}

/// function	DrawLine
/// brief		draw a line
///
/// param		x1		start x coordinate
/// param		y1		start y coordinate
/// param		x2		end x coordinate
/// param		y2		end y coordinate
/// param		color	rectangle's color
/// return		success or failed
bool SCDrawCommand::DrawLine(
		const unsigned int x1,
		const unsigned int y1,
		const unsigned int x2,
		const unsigned int y2,
		const XColor& color)
{
	bool res = true;
	
	if (x1 != x2 && y1 != y2) {
		res = false;
	} else {
		unsigned int x = min(x1, x2);
		unsigned int y = min(y1, y2);
		unsigned int width  = intABS(x1 - x2);
		unsigned int height = intABS(y1 - y2);
		if (width  == 0) ++width;
		if (height == 0) ++height;
		res = drawRect(x, y, width, height, color);
	}

	return res;
}

/// function	FillRect
/// brief		fill a rectangle
///
/// param		x		start x coordinate
/// param		y		start y coordinate
/// param		width	rectangle's width
/// param		height	rectangle's height
/// param		color	rectangle's color
/// return		success or failed
bool SCDrawCommand::FillRect(
		const unsigned int x,
		const unsigned int y,
		const unsigned int width,
		const unsigned int height,
		const XColor& color)
{
	return drawRect(x, y, width, height, color);
}

/// function	DrawRect
/// brief		draw a rectangle
///
/// param		x		start x coordinate
/// param		y		start y coordinate
/// param		width	rectangle's width
/// param		height	rectangle's height
/// param		color	rectangle's color
/// return		success or failed
bool SCDrawCommand::DrawRect(
		const unsigned int x,
		const unsigned int y,
		const unsigned int width,
		const unsigned int height,
		const XColor& fore_color,
		const XColor& back_color)
{
	bool res = FillRect(x, y, width, height, back_color);

	res &= drawRect(x, y, width, 1, fore_color);			// top
	res &= drawRect(x+width-1, y, 1, height, fore_color);	// right
	res &= drawRect(x, y+height-1, width, 1, fore_color);	// bottom
	res &= drawRect(x, y, 1, height, fore_color);			// left

	return res;
}

/// function	DrawString
/// brief		draw a string
///
/// param		x			start x coordinate
/// param		y			start y coordinate
/// param		text		string for print
/// param		fore_color	foreground color
/// param		back_color	background color
/// param		font		font type of string
/// return		success or failed
bool SCDrawCommand::DrawString(
		const unsigned int x,
		const unsigned int y,
		const unsigned short* text,
		const XColor& fore_color,
		const XColor& back_color,
		const string& font)
{
	bool res = true;
	int x_head = x;
	unsigned short* p_str = const_cast<unsigned short*>(text);

	while (*p_str) {
		res &= drawASCII(x_head, y, static_cast<const char>(*p_str),
				fore_color, back_color, font);
		p_str++;
		x_head += SC_FONT_INTERNAL;
	}

	return res;
}

/// function	DrawString
/// brief		draw a string
///
/// param		x			start x coordinate
/// param		y			start y coordinate
/// param		text		string for print
/// param		fore_color	foreground color
/// param		back_color	background color
/// param		font		font type of string
/// return		success or failed
bool SCDrawCommand::DrawString(
		const unsigned int x,
		const unsigned int y,
		const string& text,
		const XColor& fore_color,
		const XColor& back_color,
		const string& font)
{
	bool res = true;
	int x_head = x;
	char* str = const_cast<char*>(text.c_str());
	char* p_str = str;

	while (*p_str) {
		res &= drawASCII(x_head, y, static_cast<const char>(*p_str),
				fore_color, back_color, font);
		p_str++;
		x_head += SC_FONT_INTERNAL;
	}

	return res;
}

/// function	DrawPoint
/// brief		draw a point
///
/// param		p		start coordinate
/// param		color	rectangle's color
/// return		success or failed
bool SCDrawCommand::DrawPoint(
		const SCPoint& p,
		const XColor& color)
{
	return DrawPoint(p.x, p.y, color);
}

/// function	DrawLine
/// brief		draw a line
///
/// param		p1		start coordinate
/// param		p2		end coordinate
/// param		color	rectangle's color
/// return		success or failed
bool SCDrawCommand::DrawLine(
		const SCPoint& p1,
		const SCPoint& p2,
		const XColor& color)
{
	return DrawLine(p1.x, p1.y, p2.x, p2.y, color);
}

/// function	FillRect
/// brief		fill a rectangle
///
/// param		rect	rectangle's area
/// param		color	rectangle's color
/// return		success or failed
bool SCDrawCommand::FillRect(
		const SCRect& rect,
		const XColor& color)
{
	return FillRect(rect.x, rect.y, rect.width, rect.height, color);
}

/// function	DrawRect
/// brief		draw a rectangle
///
/// param		rect	rectangle's area
/// param		color	rectangle's color
/// return		success or failed
bool SCDrawCommand::DrawRect(
		const SCRect& rect,
		const XColor& fore_color,
		const XColor& back_color)
{
	return DrawRect(rect.x, rect.y, rect.width, rect.height, fore_color, back_color);
}

/// function	DrawString
/// brief		draw a string
///
/// param		x			start x coordinate
/// param		y			start y coordinate
/// param		text		string for print
/// param		fore_color	foreground color
/// param		back_color	background color
/// param		font		font type of string
/// return		success or failed
bool SCDrawCommand::DrawString(
		const SCPoint& p,
		const std::string& text,
		const XColor& fore_color,
		const XColor& back_color,
		const std::string& font)
{
	return DrawString(p.x, p.y, text, fore_color, back_color, font);
}

/// function	DrawString
/// brief		draw a string
///
/// param		x			start x coordinate
/// param		y			start y coordinate
/// param		text		string for print
/// param		fore_color	foreground color
/// param		back_color	background color
/// param		font		font type of string
/// return		success or failed
bool SCDrawCommand::DrawString(
		const SCPoint& p,
		const unsigned short* text,
		const XColor& fore_color,
		const XColor& back_color,
		const std::string& font)
{
	return DrawString(p.x, p.y, text, fore_color, back_color, font);
}

