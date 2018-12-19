///
/// file		SCColor.h
/// brief		HMI color class define head file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/12/19
///

#ifndef __SCL_COLOR_H__
#define __SCL_COLOR_H__

#include <X11/Xlib.h>

#define SC_COLOR_WHITE		SCColor::sc_white
#define SC_COLOR_BLACK		SCColor::sc_black
#define SC_COLOR_RED	 	SCColor::sc_red
#define SC_COLOR_YELLOW		SCColor::sc_yellow
#define SC_COLOR_BLUE		SCColor::sc_blue
#define SC_COLOR_GREEN		SCColor::sc_green
#define SC_COLOR_GRAY		SCColor::sc_gray
#define SC_COLOR_LIGHT_GRAY	SCColor::sc_light_gray
#define SC_COLOR_DARK_GRAY	SCColor::sc_dark_gray

///
/// class : SCColor
/// Coordinate class
///
class SCColor {
public:
	SCColor(Display* display);
	virtual ~SCColor();

public:
	void AllocColor(void);

public :
	static XColor sc_white;
	static XColor sc_black;
	static XColor sc_red;
	static XColor sc_yellow;
	static XColor sc_blue;
	static XColor sc_green;
	static XColor sc_gray;
	static XColor sc_light_gray;
	static XColor sc_dark_gray;

private:
	Display* disp;
};

#endif // __SCL_COLOR_H__

