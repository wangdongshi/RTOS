///
/// file		SCDrawContext.h
/// brief		HMI draw context class define head file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/12/07
///

#ifndef __SCL_DRAW_CONTEXT_H__
#define __SCL_DRAW_CONTEXT_H__

#include <string>
#include <X11/Xlib.h>
#include "SCBase.h"

#define SC_FONT_LARGE	(SCDrawContext::GetLargeFont())
#define SC_FONT_MIDDLE	(SCDrawContext::GetMiddleFont())
#define SC_FONT_SMALL	(SCDrawContext::GetSmallFont())

#define SC_FONT_INTERNAL	(7)

// base window size
#define WINDOW_WIDTH		(320)
#define WINDOW_HEIGHT		(240)

class SCDrawContext {
public :
	SCDrawContext();
	SCDrawContext(Display* d, Window& w);
	virtual ~SCDrawContext();

public :
	Display*		getDisplay(void) {return disp;};
	Window*			getWindow(void) {return &win;};
	GC*				getGC(void) {return &gc;};

public :
	static std::string	GetLargeFont(void)	{return font_type[0];};
	static std::string	GetMiddleFont(void)	{return font_type[1];};
	static std::string	GetSmallFont(void)	{return font_type[2];};

protected :
	bool			drawRect(const unsigned int x,
							const unsigned int y,
							const unsigned int width,
							const unsigned int height,
							const XColor& color);
	bool			drawRect(const SCPoint pos,
							const unsigned int width,
							const unsigned int height,
							const XColor& color);
	bool			drawASCII(const unsigned int x,
							const unsigned int y,
							const char ascii,
							const XColor& fore_color,
							const XColor& back_color,
							const std::string font_name);
	bool			drawASCII(const SCPoint pos,
							const char ascii,
							const XColor& fore_color,
							const XColor& back_color,
							const std::string font_name);
	
protected :
	Display*		disp;
	Window			win;
	GC				gc;

	static const std::string font_type[3];
};

#endif // __SCL_DRAW_CONTEXT_H__

