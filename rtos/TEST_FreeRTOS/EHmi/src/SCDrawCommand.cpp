/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename:  SCDrawCommand.cpp
 * Project:   Minimum RTOS platform
 * Date:      2018/11/26
 * Author:    WangYu
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

uint32_t SCDrawCommand::GetStringWidth(const uint16_t* str, const uint32_t font)
{
	uint32_t	width = 0;

	do {
		uint32_t i = static_cast<uint32_t>(*str);
		width += (SC_FONT_SMALL == font) ? sFont.st.cdef[i].width : mFont.st.cdef[i].width;
	} while (*str++);

	return width;
}

bool SCDrawCommand::DrawPoint(
		const uint32_t x,
		const uint32_t y,
		const uint32_t color)
{
	return drawRect(x, y, 1, 1, color);
}

bool SCDrawCommand::DrawLine(
		const uint32_t x1,
		const uint32_t y1,
		const uint32_t x2,
		const uint32_t y2,
		const uint32_t color)
{
	bool res = true;
	
	if (x1 != x2 && y1 != y2) {
		res = false;
	} else {
		uint32_t x = std::min(x1, x2);
		uint32_t y = std::min(y1, y2);
		uint32_t width  = std::fabs(x1 - x2);
		uint32_t height = std::fabs(y1 - y2);
		if (width  == 0) ++width;
		if (height == 0) ++height;
		res = drawRect(x, y, width, height, color);
	}

	return res;
}

bool SCDrawCommand::FillRect(
		const uint32_t x,
		const uint32_t y,
		const uint32_t width,
		const uint32_t height,
		const uint32_t color)
{
	return drawRect(x, y, width, height, color);
}

bool SCDrawCommand::DrawRect(
		const uint32_t x,
		const uint32_t y,
		const uint32_t width,
		const uint32_t height,
		const uint32_t fore_color,
		const uint32_t back_color)
{
	bool res = FillRect(x, y, width, height, back_color);

	res &= drawRect(x, y, width, 1, fore_color);				// top
	res &= drawRect(x + width - 1, y, 1, height, fore_color);	// right
	res &= drawRect(x, y + height - 1, width, 1, fore_color);	// bottom
	res &= drawRect(x, y, 1, height, fore_color);				// left

	return res;
}

bool SCDrawCommand::DrawString(
		const uint32_t x,
		const uint32_t y,
		const uint16_t* text,
		const uint32_t fore_color,
		const uint32_t back_color,
		const uint32_t font)
{
	bool		res = true;
	uint16_t*	p_str = const_cast<uint16_t*>(text);
	uint32_t	xHead = x;

	while (*p_str) {
		char ascii = static_cast<const char>(*p_str);
		uint32_t i = static_cast<uint32_t>(*p_str);
		res &= drawASCII(xHead, y, ascii, fore_color, back_color, font);
		xHead += (SC_FONT_SMALL == font) ? sFont.st.cdef[i].width : mFont.st.cdef[i].width;
		p_str++;
	}

	return res;
}

bool SCDrawCommand::DrawString(
		const uint32_t x,
		const uint32_t y,
		const std::string text,
		const uint32_t fore_color,
		const uint32_t back_color,
		const uint32_t font)
{
	bool		res = true;
	char*		str = const_cast<char*>(text.c_str());
	char*		p_str = str;
	uint32_t	xHead = x;

	while (*p_str) {
		char ascii = static_cast<const char>(*p_str);
		uint32_t i = static_cast<uint32_t>(*p_str);
		res &= drawASCII(xHead, y, ascii, fore_color, back_color, font);
		xHead += (SC_FONT_SMALL == font) ? sFont.st.cdef[i].width : mFont.st.cdef[i].width;
		p_str++;
	}

	return res;
}

bool SCDrawCommand::DrawPoint(
		const SCPoint& p,
		const uint32_t color)
{
	return DrawPoint(p.x, p.y, color);
}

bool SCDrawCommand::DrawLine(
		const SCPoint& p1,
		const SCPoint& p2,
		const uint32_t color)
{
	return DrawLine(p1.x, p1.y, p2.x, p2.y, color);
}

bool SCDrawCommand::FillRect(
		const SCRect& rect,
		const uint32_t color)
{
	return FillRect(rect.x, rect.y, rect.width, rect.height, color);
}

bool SCDrawCommand::DrawRect(
		const SCRect& rect,
		const uint32_t fore_color,
		const uint32_t back_color)
{
	return DrawRect(rect.x, rect.y, rect.width, rect.height, fore_color, back_color);
}

bool SCDrawCommand::DrawString(
		const SCPoint& p,
		const uint16_t* text,
		const uint32_t fore_color,
		const uint32_t back_color,
		const uint32_t font)
{
	return DrawString(p.x, p.y, text, fore_color, back_color, font);
}

bool SCDrawCommand::DrawString(
		const SCPoint& p,
		const std::string text,
		const uint32_t fore_color,
		const uint32_t back_color,
		const uint32_t font)
{
	return DrawString(p.x, p.y, text, fore_color, back_color, font);
}
