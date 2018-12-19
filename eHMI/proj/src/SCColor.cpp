///
/// file		SCColor.cpp
/// brief		HMI color class source file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/12/19
///
#include "SCColor.h"

XColor SCColor::sc_white;
XColor SCColor::sc_black;
XColor SCColor::sc_red;
XColor SCColor::sc_yellow;
XColor SCColor::sc_blue;
XColor SCColor::sc_green;
XColor SCColor::sc_gray;
XColor SCColor::sc_light_gray;
XColor SCColor::sc_dark_gray;

/// function	SCColor
/// brief		constructor
SCColor::SCColor(Display* display) : disp(display)
{
}

/// function	~SCColor
/// brief		deconstructor
SCColor::~SCColor()
{
}

/// function	AllocColor
/// brief		alloc color for eHMI
void SCColor::AllocColor(void)
{
	Colormap cmap;
	XColor true_color;
	cmap = DefaultColormap(disp, 0);
	XAllocNamedColor(disp, cmap, "White",		&sc_white,		&true_color);
	XAllocNamedColor(disp, cmap, "Black",		&sc_black,		&true_color);
	XAllocNamedColor(disp, cmap, "Blue",		&sc_blue,		&true_color);
	XAllocNamedColor(disp, cmap, "Red",			&sc_red,		&true_color);
	XAllocNamedColor(disp, cmap, "Yellow",		&sc_yellow,		&true_color);
	XAllocNamedColor(disp, cmap, "Blue",		&sc_blue,		&true_color);
	XAllocNamedColor(disp, cmap, "Green",		&sc_green,		&true_color);
	XAllocNamedColor(disp, cmap, "Gray",		&sc_gray,		&true_color);
	XAllocNamedColor(disp, cmap, "LightGray",	&sc_light_gray,	&true_color);
	XAllocNamedColor(disp, cmap, "DarkGray",	&sc_dark_gray,	&true_color);
}
