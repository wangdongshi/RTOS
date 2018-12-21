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

#include <map>
#include <X11/Xlib.h>

#define SC_COLOR_NUMBER		30	// there is margin
#define SC_COLOR(name)		SCColor::GetColor(name)

///
/// class : SCColor
/// Coordinate class
///
class SCColor {
public :
	SCColor();
	virtual ~SCColor();

	static XColor GetColor(std::string color_name);
	static void AllocColor(void);
	
private :
	static std::map<std::string, int> color_map; // color index in xcolor
	static XColor	xcolor[SC_COLOR_NUMBER];
};

#endif // __SCL_COLOR_H__

