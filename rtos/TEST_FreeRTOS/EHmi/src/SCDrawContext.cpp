/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : SCDrawContext.cpp
 * Project  : Minimum RTOS platform
 * Date     : 2018/11/26
 * Author   : WangYu
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

bool SCDrawContext::Initialize(void)
{
	fillRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SC_COLOR_TRANS,      LAYER_FG);
	fillRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SC_COLOR_LIGHT_GRAY, LAYER_BG);

	return true;
}

bool SCDrawContext::drawRect(
		const unsigned int x,
		const unsigned int y,
		const unsigned int width,
		const unsigned int height,
		const unsigned int color)
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
		const unsigned int width,
		const unsigned int height,
		const unsigned int color)
{
	return drawRect(pos.x, pos.y, width, height, color);
}

bool SCDrawContext::drawASCII(
		const unsigned int x,
		const unsigned int y,
		const char ascii,
		const unsigned int fore_color,
		const unsigned int back_color,
		const unsigned int font)
{
	assert_param(WINDOW_WIDTH  > x);
	assert_param(WINDOW_HEIGHT > y);
	assert_param(ascii < 128);
	assert_param(font < 2);
	drawChar(x, y, fore_color, back_color, ascii, static_cast<FONT_TYPE>(font), LAYER_FG);
	return true;
}

bool SCDrawContext::drawASCII(
		const SCPoint pos,
		const char ascii,
		const unsigned int fore_color,
		const unsigned int back_color,
		const unsigned int font)
{
	return drawASCII(pos.x, pos.y, ascii, fore_color, back_color, static_cast<FONT_TYPE>(font));
}

