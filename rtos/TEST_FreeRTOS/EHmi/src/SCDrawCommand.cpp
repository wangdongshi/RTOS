/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : SCDrawCommand.cpp
 * Project  : Minimum RTOS platform
 * Date     : 2018/11/26
 * Author   : WangYu
 *
 **********************************************************************/
#include <math.h>
#include <string>
#include "SCDrawCommand.h"

SCDrawCommand::SCDrawCommand()
{
}

SCDrawCommand::~SCDrawCommand()
{
}

unsigned int SCDrawCommand::GetStringWidth(const unsigned short* str, const unsigned int font)
{
	unsigned int width = 0;

	while (*str) {
		unsigned int i = static_cast<unsigned int>(*str);
		width += (SC_FONT_SMALL == font) ? sFont.st.cdef[i].width : mFont.st.cdef[i].width;
		str++;
	}

	return width;
}

time_t SCDrawCommand::GetSysTime()
{
	static unsigned int add = 0;
	time_t sys_time;

	time(&sys_time);
	sys_time += 8*3600 + add++;  // transform the time zone

	return sys_time;
}

bool SCDrawCommand::DrawPoint(
		const unsigned int x,
		const unsigned int y,
		const unsigned int color)
{
	return drawRect(x, y, 1, 1, color);
}

bool SCDrawCommand::DrawLine(
		const unsigned int x1,
		const unsigned int y1,
		const unsigned int x2,
		const unsigned int y2,
		const unsigned int color)
{
	bool res = true;
	
	if (x1 != x2 && y1 != y2) {
		res = false;
	} else {
		unsigned int x = std::min(x1, x2);
		unsigned int y = std::min(y1, y2);
		unsigned int width  = (unsigned int)(std::fabs((int32_t)x1 - (int32_t)x2));
		unsigned int height = (unsigned int)(std::fabs((int32_t)y1 - (int32_t)y2));
		if (width  == 0) ++width;
		if (height == 0) ++height;
		res = drawRect(x, y, width, height, color);
	}

	return res;
}

bool SCDrawCommand::FillRect(
		const unsigned int x,
		const unsigned int y,
		const unsigned int width,
		const unsigned int height,
		const unsigned int color)
{
	return drawRect(x, y, width, height, color);
}

bool SCDrawCommand::DrawRect(
		const unsigned int x,
		const unsigned int y,
		const unsigned int width,
		const unsigned int height,
		const unsigned int fore_color,
		const unsigned int back_color)
{
	bool res = FillRect(x, y, width, height, back_color);

	res &= drawRect(x, y, width, 1, fore_color);				// top
	res &= drawRect(x + width - 1, y, 1, height, fore_color);	// right
	res &= drawRect(x, y + height - 1, width, 1, fore_color);	// bottom
	res &= drawRect(x, y, 1, height, fore_color);				// left

	return res;
}

bool SCDrawCommand::DrawString(
		const unsigned int x,
		const unsigned int y,
		const unsigned short* text,
		const unsigned int fore_color,
		const unsigned int back_color,
		const unsigned int font)
{
	bool			res 	= true;
	unsigned short*	p_str 	= const_cast<unsigned short*>(text);
	unsigned int	xHead 	= x;

	while (*p_str) {
		char ascii = static_cast<const char>(*p_str);
		unsigned int i = static_cast<unsigned int>(*p_str);
		res &= drawASCII(xHead, y, ascii, fore_color, back_color, font);
		xHead += (SC_FONT_SMALL == font) ? sFont.st.cdef[i].width : mFont.st.cdef[i].width;
		p_str++;
	}

	return res;
}

bool SCDrawCommand::DrawString(
		const unsigned int x,
		const unsigned int y,
		const std::string text,
		const unsigned int fore_color,
		const unsigned int back_color,
		const unsigned int font)
{
	bool			res = true;
	char*			str = const_cast<char*>(text.c_str());
	char*			p_str = str;
	unsigned int	xHead = x;

	while (*p_str) {
		char ascii = static_cast<const char>(*p_str);
		unsigned int i = static_cast<unsigned int>(*p_str);
		res &= drawASCII(xHead, y, ascii, fore_color, back_color, font);
		xHead += (SC_FONT_SMALL == font) ? sFont.st.cdef[i].width : mFont.st.cdef[i].width;
		p_str++;
	}

	return res;
}

bool SCDrawCommand::DrawPoint(
		const SCPoint& p,
		const unsigned int color)
{
	return DrawPoint(p.x, p.y, color);
}

bool SCDrawCommand::DrawLine(
		const SCPoint& p1,
		const SCPoint& p2,
		const unsigned int color)
{
	return DrawLine(p1.x, p1.y, p2.x, p2.y, color);
}

bool SCDrawCommand::FillRect(
		const SCRect& rect,
		const unsigned int color)
{
	return FillRect(rect.x, rect.y, rect.width, rect.height, color);
}

bool SCDrawCommand::DrawRect(
		const SCRect& rect,
		const unsigned int fore_color,
		const unsigned int back_color)
{
	return DrawRect(rect.x, rect.y, rect.width, rect.height, fore_color, back_color);
}

bool SCDrawCommand::DrawString(
		const SCPoint& p,
		const unsigned short* text,
		const unsigned int fore_color,
		const unsigned int back_color,
		const unsigned int font)
{
	return DrawString(p.x, p.y, text, fore_color, back_color, font);
}

bool SCDrawCommand::DrawString(
		const SCPoint& p,
		const std::string text,
		const unsigned int fore_color,
		const unsigned int back_color,
		const unsigned int font)
{
	return DrawString(p.x, p.y, text, fore_color, back_color, font);
}
