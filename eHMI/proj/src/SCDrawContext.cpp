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
#include "SCDrawContext.h"

const char SCDrawContext::font_type[3][70] = {
	"6x13",
	"6x13",
	"6x13",
};

/// function	SCDrawContext
/// brief		constructor
SCDrawContext::SCDrawContext()
{
}

/// function	SCDrawContext
/// brief		constructor
SCDrawContext::SCDrawContext(Display* display, Window& window) : 
disp(display),
win(window)
{
	gc = XCreateGC(disp, win, 0, NULL);
}

/// function	~SCDrawContext
/// brief		deconstructor
SCDrawContext::~SCDrawContext()
{
	XFreeGC(disp, gc);
}

/// function	drawRect
/// brief		draw a rectangle
///
/// param		x		start x coordinate
/// param		y		start y coordinate
/// param		width	rectangle's width
/// param		height	rectangle's height
/// param		color	rectangle's color
/// return		none
void SCDrawContext::drawRect(
		const unsigned int x,
		const unsigned int y,
		const unsigned int width,
		const unsigned int height,
		const unsigned long color) {
	Colormap map = allocColor(color);
	XFillRectangle(disp, win, gc, x, y, width, height);
	XFreeColormap(disp, map);
};

/// function	drawRect
/// brief		draw a rectangle
///
/// param		pos		start coordinate
/// param		width	rectangle's width
/// param		height	rectangle's height
/// param		color	rectangle's color
/// return		none
void SCDrawContext::drawRect(
		const SCPos pos,
		const unsigned int width,
		const unsigned int height,
		const unsigned long color) {
	drawRect(pos.x, pos.y, width, height, color);
};

/// function	drawASCII
/// brief		draw a ASCII charactor
///
/// param		x		start x coordinate
/// param		y		start y coordinate
/// param		ascii	charactor
/// param		fore_color	charactor's color
/// param		back_color	charactor's background color
/// param		font_name	font name of charactor
/// return		none
void SCDrawContext::drawASCII(
		const unsigned int x,
		const unsigned int y,
		const char ascii,
		const unsigned long fore_color,
		const unsigned long back_color,
		char* font_name) {
	Font font = XLoadFont(disp, font_name);
	if (!font)
	{
		Trace("XLoadFont Error!");
	} else {
		Colormap fmap = allocColor(fore_color, true);
		//Colormap bmap = allocColor(back_color, false);
		XSetFont(disp, gc, font);
		XTextItem text = {const_cast<char*>(&ascii), 1, 0, font};
		XDrawText(disp, win, gc, x, y, &text, 1);
		XUnloadFont(disp, font);
		XFreeColormap(disp, fmap);
		//XFreeColormap(disp, bmap);
		//XFreeFont(disp, &font);
	}
};

/// function	drawASCII
/// brief		draw a ASCII charactor
///
/// param		pos		start coordinate
/// param		ascii	charactor
/// param		fore_color	charactor's color
/// param		back_color	charactor's background color
/// param		font_name	font name of charactor
/// return		none
void SCDrawContext::drawASCII(
		const SCPos pos,
		const char ascii,
		const unsigned long fore_color,
		const unsigned long back_color,
		char* font_name) {
	drawASCII(pos.x, pos.y, ascii, fore_color, back_color, font_name);
};

/// function	allocColor
/// brief		alloc color by RGB value
///
/// param		rgb		RGB value
/// param		fb		true:foreground, false:background
/// return		none
Colormap SCDrawContext::allocColor(
		const unsigned long rgb,
		const bool fb) {
	Status res;
	XColor xcolor;
	Colormap map = DefaultColormap(disp, DefaultScreen(disp));
	xcolor.red   = (rgb & 0x0000FFFF00000000) >> 32;
	xcolor.green = (rgb & 0x00000000FFFF0000) >> 16;
	xcolor.blue  = (rgb & 0x000000000000FFFF);
	res = XAllocColor(disp, map, &xcolor);
	if (res) {
		if (fb)	XSetForeground(disp, gc, xcolor.pixel);
		else 	XSetBackground(disp, gc, xcolor.pixel);
	} else {
		Trace("Color allocation is failed!\n");
	}

	return map;
}

	
