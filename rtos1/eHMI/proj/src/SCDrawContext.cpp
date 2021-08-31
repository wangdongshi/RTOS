///
/// file		SCDrawContext.cpp
/// brief		HMI draw context class source file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/12/10
///
#include <unistd.h>
#include <termio.h>
#include "debug.h"
#include "SCColor.h"
#include "SCDrawContext.h"

using namespace std;

// static parameter initialization
Display*	 SCDrawContext::disp;
Window*		 SCDrawContext::win;
GC*			 SCDrawContext::gc;
const string SCDrawContext::font_type[3] = {
	"6x13",
	"6x13",
	"6x13",
};

/// function	SCDrawContext
/// brief		constructor
SCDrawContext::SCDrawContext()
{
}

/// function	~SCDrawContext
/// brief		deconstructor
SCDrawContext::~SCDrawContext()
{
}

/// function	Initialize
/// brief		bonding draw resource
///
/// param		d		display
/// param		w		window
/// param		g		GC
/// return		none
void SCDrawContext::Initialize(Display* d, Window* w, GC* g)
{
	disp = d;
	win	 = w;
	gc	 = g;
}

/// function	drawRect
/// brief		draw a rectangle
///
/// param		x		start x coordinate
/// param		y		start y coordinate
/// param		width	rectangle's width
/// param		height	rectangle's height
/// param		color	rectangle's color
/// return		success or failed
bool SCDrawContext::drawRect(
		const unsigned int x,
		const unsigned int y,
		const unsigned int width,
		const unsigned int height,
		const XColor& color)
{
	if (x+width > WINDOW_WIDTH || y+height > WINDOW_HEIGHT) return false;

	if (!IsTrans(color)) {
		XSetForeground(disp, *gc, color.pixel);
		XFillRectangle(disp, *win, *gc, x, y, width, height);
		XFlush(disp);
	}
	return true;
}

/// function	drawRect
/// brief		draw a rectangle
///
/// param		pos		start coordinate
/// param		width	rectangle's width
/// param		height	rectangle's height
/// param		color	rectangle's color
/// return		success or failed
bool SCDrawContext::drawRect(
		const SCPoint pos,
		const unsigned int width,
		const unsigned int height,
		const XColor& color)
{
	return drawRect(pos.x, pos.y, width, height, color);
}

/// function	drawASCII
/// brief		draw a ASCII charactor
///
/// param		x		start x coordinate
/// param		y		start y coordinate
/// param		ascii	charactor
/// param		fore_color	charactor's color
/// param		back_color	charactor's background color
/// param		font_name	font name of charactor
/// return		success or failed
bool SCDrawContext::drawASCII(
		const unsigned int x,
		const unsigned int y,
		const char ascii,
		const XColor& fore_color,
		const XColor& back_color,
		const string font_name)
{
	//Trace("Foreground color = Red:%x, Green:%x, Blue:%x", fore_color.red, fore_color.green, fore_color.blue);
	//Trace("Background color = Red:%x, Green:%x, Blue:%x", back_color.red, back_color.green, back_color.blue);
	// Fill background (Null : transparent)
	if (!IsTrans(back_color)) {
		XSetForeground(disp, *gc, back_color.pixel);
		XFillRectangle(disp, *win, *gc, x, y, SC_FONT_INTERNAL, 13);
	}

	// Draw string
	if (!IsTrans(fore_color)) {
		Font font = XLoadFont(disp, font_name.c_str());
		if (!font)
		{
			Trace("XLoadFont Error!");
			return false;
		} else {
			if (x+6 > WINDOW_WIDTH || y+11 > WINDOW_HEIGHT) return false;
			XSetForeground(disp, *gc, fore_color.pixel);
			//XSetBackground(disp, *gc, back_color.pixel);
			XSetFont(disp, *gc, font);
			XTextItem text = {const_cast<char*>(&ascii), 1, 0, font};
			XDrawText(disp, *win, *gc, x, y+11, &text, 1);
			XUnloadFont(disp, font);
			XFlush(disp);
			return true;
		}
	} else {
		return true;
	}
}

/// function	drawASCII
/// brief		draw a ASCII charactor
///
/// param		pos		start coordinate
/// param		ascii	charactor
/// param		fore_color	charactor's color
/// param		back_color	charactor's background color
/// param		font_name	font name of charactor
/// return		success or failed
bool SCDrawContext::drawASCII(
		const SCPoint pos,
		const char ascii,
		const XColor& fore_color,
		const XColor& back_color,
		const string font_name)
{
	return drawASCII(pos.x, pos.y, ascii, fore_color, back_color, font_name);
}

