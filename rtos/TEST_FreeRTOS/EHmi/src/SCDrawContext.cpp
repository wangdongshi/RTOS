/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename:  SCDrawContext.cpp
 * Project:   Minimum RTOS platform
 * Date:      2018/11/26
 * Author:    WangYu
 *
 **********************************************************************/
#include "assert_param.h"
#include "SCDrawContext.h"

SCDrawContext::SCDrawContext()
{
}

SCDrawContext::~SCDrawContext()
{
}

void SCDrawContext::Initialize(void)
{
	fillRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SC_COLOR_TRANS,      LAYER_FG);
	fillRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SC_COLOR_LIGHT_GRAY, LAYER_BG);
}

bool SCDrawContext::drawRect(
		const uint32_t x,
		const uint32_t y,
		const uint32_t width,
		const uint32_t height,
		const uint32_t color)
{
	assert_param(WINDOW_WIDTH  >= x + width);
	assert_param(WINDOW_HEIGHT >= y + height);

	if (!IsTrans(color)) {
		fillRect(x, y, width, height, color, LAYER_BG);
	}
	return true;
}

bool SCDrawContext::drawRect(
		const SCPoint pos,
		const uint32_t width,
		const uint32_t height,
		const uint32_t color)
{
	return drawRect(pos.x, pos.y, width, height, color);
}

bool SCDrawContext::drawASCII(
		const uint32_t x,
		const uint32_t y,
		const char ascii,
		const uint32_t foreColor,
		const uint32_t backColor,
		const uint32_t font)
{
	assert_param(WINDOW_WIDTH  > x);
	assert_param(WINDOW_HEIGHT > y);
	assert_param(ascii < 128);
	assert_param(font < 2);
	drawChar(x, y, foreColor, backColor, ascii, font, LAYER_FG);
	return true;
}

bool SCDrawContext::drawASCII(
		const SCPoint pos,
		const char ascii,
		const uint32_t foreColor,
		const uint32_t backColor,
		const uint32_t font)
{
	return drawASCII(pos.x, pos.y, ascii, foreColor, backColor, font);
}

