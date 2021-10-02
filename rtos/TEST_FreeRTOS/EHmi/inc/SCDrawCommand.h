/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename:  SCDrawCommand.h
 * Project:   Minimum RTOS platform
 * Date:      2018/11/26
 * Author:    WangYu
 *
 **********************************************************************/
#ifndef __SCL_DRAW_COMMAND_H__
#define __SCL_DRAW_COMMAND_H__

#include <string>
#include "font.h"
#include "SCDrawContext.h"

#define InitializeScreen	SCDrawCommand::Initialize
#define GetStrWidth			SCDrawCommand::GetStringWidth
#define MakePoint			SCDrawCommand::DrawPoint
#define MakeLine			SCDrawCommand::DrawLine
#define PaintRect			SCDrawCommand::FillRect
#define MakeRect			SCDrawCommand::DrawRect
#define MakeString			SCDrawCommand::DrawString

#define ClearRect(r)		SCDrawCommand::FillRect(r)

class SCDrawCommand : public SCDrawContext {
public:
	SCDrawCommand();
	virtual ~SCDrawCommand();

	static uint32_t	GetStringWidth(const uint16_t* str, const uint32_t font);

	static bool		DrawPoint(const uint32_t x,
							const uint32_t y,
							const uint32_t color = SC_COLOR_BLACK);
	static bool		DrawLine(const uint32_t x1,
							const uint32_t y1,
							const uint32_t x2,
							const uint32_t y2,
							const uint32_t color = SC_COLOR_BLACK);
	static bool		FillRect(const uint32_t x,
							const uint32_t y,
							const uint32_t width,
							const uint32_t height,
							const uint32_t color);
	static bool		DrawRect(const uint32_t x,
							const uint32_t y,
							const uint32_t width,
							const uint32_t height,
							const uint32_t foreColor = SC_COLOR_BLACK,
							const uint32_t backColor = SC_COLOR_TRANS);
	static bool		DrawString(const uint32_t x,
							const uint32_t y,
							const uint16_t* text,
							const uint32_t foreColor = SC_COLOR_BLACK,
							const uint32_t backColor = SC_COLOR_TRANS,
							const uint32_t font = SC_FONT_MIDDLE);
	static bool		DrawString(const uint32_t x,
							const uint32_t y,
							const std::string text,
							const uint32_t foreColor = SC_COLOR_BLACK,
							const uint32_t backColor = SC_COLOR_TRANS,
							const uint32_t font = SC_FONT_MIDDLE);
								
	static bool		DrawPoint(const SCPoint& p,
							const uint32_t color = SC_COLOR_BLACK);
	static bool		DrawLine(const SCPoint& p1,
							const SCPoint& p2,
							const uint32_t color = SC_COLOR_BLACK);
	static bool		FillRect(const SCRect& rect,
							const uint32_t color);
	static bool		DrawRect(const SCRect& rect,
							const uint32_t foreColor = SC_COLOR_BLACK,
							const uint32_t backColor = SC_COLOR_TRANS);
	static bool		DrawString(const SCPoint& p,
							const uint16_t* text,
							const uint32_t foreColor = SC_COLOR_BLACK,
							const uint32_t backColor = SC_COLOR_TRANS,
							const uint32_t font = SC_FONT_MIDDLE);
	static bool		DrawString(const SCPoint& p,
							const std::string text,
							const uint32_t foreColor = SC_COLOR_BLACK,
							const uint32_t backColor = SC_COLOR_TRANS,
							const uint32_t font = SC_FONT_MIDDLE);
};

#endif // __SCL_DRAW_COMMAND_H__

