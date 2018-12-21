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

#include "SCColor.h"
#include "SCDrawContext.h"

#define MakePoint	SCDrawCommand::DrawPoint
#define MakeLine	SCDrawCommand::DrawLine
#define MakeRect	SCDrawCommand::DrawRect
#define MakeString	SCDrawCommand::DrawString

class SCDrawCommand : public SCDrawContext {
public:
	SCDrawCommand();
	virtual ~SCDrawCommand();

	static bool		DrawPoint(const unsigned int x,
							const unsigned int y,
							const XColor& color = SC_COLOR("Black"));
	static bool		DrawLine(const unsigned int x1,
							const unsigned int y1,
							const unsigned int x2,
							const unsigned int y2,
							const XColor& color = SC_COLOR("Black"));
	static bool		FillRect(const unsigned int x,
							const unsigned int y,
							const unsigned int width,
							const unsigned int height,
							const XColor& color = SC_COLOR("Gray"));
	static bool		DrawRect(const unsigned int x,
							const unsigned int y,
							const unsigned int width,
							const unsigned int height,
							const XColor& fore_color = SC_COLOR("Black"),
							const XColor& back_color = SC_COLOR("LightGray"));
	static bool		DrawString(const unsigned int x,
							const unsigned int y,
							const std::string text,
							const std::string font = SC_FONT_MIDDLE,
							const XColor& fore_color = SC_COLOR("Black"),
							const XColor& back_color = SC_COLOR("LightGray"));
};

#endif // __SCL_DRAW_COMMAND_H__

