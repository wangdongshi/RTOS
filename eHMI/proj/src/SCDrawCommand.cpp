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
#include "SCDrawCommand.h"

using namespace std;

/// function	SCDrawCommand
/// brief		constructor
SCDrawCommand::SCDrawCommand()
{
}

/// function	SCDrawCommand
/// brief		constructor
SCDrawCommand::SCDrawCommand(Display* d, Window& w) :
SCDrawContext(d, w),
pcolor(new SCColor(d))
{
	pcolor->AllocColor();
}

/// function	~SCDrawContext
/// brief		deconstructor
SCDrawCommand::~SCDrawCommand()
{
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
/// param		x1		end x coordinate
/// param		y1		end y coordinate
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
		unsigned int width  = fabs(x1 - x2);
		unsigned int height = fabs(y1 - y2);
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
		const XColor& color)
{
	bool res = true;

	res &= drawRect(x, y, width, 1, color);			// top
	res &= drawRect(x+width-1, y, 1, height, color);// right
	res &= drawRect(x, y+height-1, width, 1, color);// bottom
	res &= drawRect(x, y, 1, height, color);		// left

	return res;
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
bool SCDrawCommand::DrawString(
		const unsigned int x,
		const unsigned int y,
		const string text,
		const string font,
		const XColor& fore_color,
		const XColor& back_color)
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

