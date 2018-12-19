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

#include <unordered_map>
#include <X11/Xlib.h>

#define SC_COLOR_NUMBER		30	// there is margin

///
/// class : SCColor
/// Coordinate class
///
class SCColor {
public :
	SCColor(Display* display);
	virtual ~SCColor();

public :
	void AllocColor(void);
	XColor GetColor(char* p_color);
	
public :
	static std::unordered_map<int, std::string> colorMap;	// color name's index

private :
	static XColor xcolor[SC_COLOR_NUMBER];
	Display* disp;
	Colormap cmap;
};

#endif // __SCL_COLOR_H__
