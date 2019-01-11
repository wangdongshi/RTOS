///
/// file		SCDrawCommand.h
/// brief		HMI draw command class define head file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/12/19
///

#ifndef __SCL_DRAW_COMMAND_H__
#define __SCL_DRAW_COMMAND_H__

#include <time.h>
#include "SCColor.h"
#include "SCDrawContext.h"

#define GetTime			SCDrawCommand::GetSysTime
#define GetStrWidth		SCDrawCommand::GetStringWidth
#define MakePoint		SCDrawCommand::DrawPoint
#define MakeLine		SCDrawCommand::DrawLine
#define PaintRect		SCDrawCommand::FillRect
#define MakeRect		SCDrawCommand::DrawRect
#define MakeString		SCDrawCommand::DrawString

#define ClearRect(r)	SCDrawCommand::FillRect(r)

#define intABS(x)		(((int)(x) > 0) ? ((int)(x)) : (-(int)(x)))

class SCDrawCommand : public SCDrawContext {
public:
	SCDrawCommand();
	virtual ~SCDrawCommand();
	
	static int		GetStringWidth(const unsigned short* str);
	static time_t	GetSysTime(void);

	static bool		DrawPoint(const unsigned int x,
							const unsigned int y,
							const XColor& color = COLOR(Black));
	static bool		DrawLine(const unsigned int x1,
							const unsigned int y1,
							const unsigned int x2,
							const unsigned int y2,
							const XColor& color = COLOR(Black));
	static bool		FillRect(const unsigned int x,
							const unsigned int y,
							const unsigned int width,
							const unsigned int height,
							const XColor& color);
	static bool		DrawRect(const unsigned int x,
							const unsigned int y,
							const unsigned int width,
							const unsigned int height,
							const XColor& fore_color = COLOR(Black),
							const XColor& back_color = COLOR(Transparent));
	static bool		DrawString(const unsigned int x,
							const unsigned int y,
							const std::string& text,
							const XColor& fore_color = COLOR(Black),
							const XColor& back_color = COLOR(Transparent),
							const std::string& font = SC_FONT_MIDDLE);
	static bool		DrawString(const unsigned int x,
							const unsigned int y,
							const unsigned short* text,
							const XColor& fore_color = COLOR(Black),
							const XColor& back_color = COLOR(Transparent),
							const std::string& font = SC_FONT_MIDDLE);
								
	static bool		DrawPoint(const SCPoint& p,
							const XColor& color = COLOR(Black));
	static bool		DrawLine(const SCPoint& p1,
							const SCPoint& p2,
							const XColor& color = COLOR(Black));
	static bool		FillRect(const SCRect& rect,
							const XColor& color);
	static bool		DrawRect(const SCRect& rect,
							const XColor& fore_color = COLOR(Black),
							const XColor& back_color = COLOR(Transparent));
	static bool		DrawString(const SCPoint& p,
							const std::string& text,
							const XColor& fore_color = COLOR(Black),
							const XColor& back_color = COLOR(Transparent),
							const std::string& font = SC_FONT_MIDDLE);
	static bool		DrawString(const SCPoint& p,
							const unsigned short* text,
							const XColor& fore_color = COLOR(Black),
							const XColor& back_color = COLOR(Transparent),
							const std::string& font = SC_FONT_MIDDLE);
};

#endif // __SCL_DRAW_COMMAND_H__

