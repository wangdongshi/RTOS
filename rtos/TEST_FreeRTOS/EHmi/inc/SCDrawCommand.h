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
#include <time.h>
#include "font.h"
#include "SCDrawContext.h"

#define InitializeLCD		SCDrawCommand::Initialize
#define GetTime				SCDrawCommand::GetSysTime
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

	static unsigned int	GetStringWidth(const unsigned short* str, const unsigned int font);
	static time_t	GetSysTime(void);

	static bool		DrawPoint(const unsigned int x,
							const unsigned int y,
							const unsigned int color = SC_COLOR_BLACK);
	static bool		DrawLine(const unsigned int x1,
							const unsigned int y1,
							const unsigned int x2,
							const unsigned int y2,
							const unsigned int color = SC_COLOR_BLACK);
	static bool		FillRect(const unsigned int x,
							const unsigned int y,
							const unsigned int width,
							const unsigned int height,
							const unsigned int color);
	static bool		DrawRect(const unsigned int x,
							const unsigned int y,
							const unsigned int width,
							const unsigned int height,
							const unsigned int fore_color = SC_COLOR_BLACK,
							const unsigned int back_color = SC_COLOR_TRANS);
	static bool		DrawString(const unsigned int x,
							const unsigned int y,
							const unsigned short* text,
							const unsigned int fore_color = SC_COLOR_BLACK,
							const unsigned int back_color = SC_COLOR_TRANS,
							const unsigned int font = SC_FONT_MIDDLE);
	static bool		DrawString(const unsigned int x,
							const unsigned int y,
							const std::string text,
							const unsigned int fore_color = SC_COLOR_BLACK,
							const unsigned int back_color = SC_COLOR_TRANS,
							const unsigned int font = SC_FONT_MIDDLE);
								
	static bool		DrawPoint(const SCPoint& p,
							const unsigned int color = SC_COLOR_BLACK);
	static bool		DrawLine(const SCPoint& p1,
							const SCPoint& p2,
							const unsigned int color = SC_COLOR_BLACK);
	static bool		FillRect(const SCRect& rect,
							const unsigned int color);
	static bool		DrawRect(const SCRect& rect,
							const unsigned int fore_color = SC_COLOR_BLACK,
							const unsigned int back_color = SC_COLOR_TRANS);
	static bool		DrawString(const SCPoint& p,
							const unsigned short* text,
							const unsigned int fore_color = SC_COLOR_BLACK,
							const unsigned int back_color = SC_COLOR_TRANS,
							const unsigned int font = SC_FONT_MIDDLE);
	static bool		DrawString(const SCPoint& p,
							const std::string text,
							const unsigned int fore_color = SC_COLOR_BLACK,
							const unsigned int back_color = SC_COLOR_TRANS,
							const unsigned int font = SC_FONT_MIDDLE);
};

#endif // __SCL_DRAW_COMMAND_H__

