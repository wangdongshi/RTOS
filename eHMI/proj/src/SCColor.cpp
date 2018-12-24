///
/// file		SCColor.cpp
/// brief		HMI color class source file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/12/19
///
#include "debug.h"
#include "SCColor.h"
#include "SCDrawContext.h"

using namespace std;

map<string, int> SCColor::color_map = {
	{"Chocolate",	0	}, // transparent
	{"White",		1	},
	{"Black",		2	},
	{"Red",			3	},
	{"Yellow",		4	},
	{"Blue",		5	},
	{"Green",		6	},
	{"Gray",		7	},
	{"LightGray",	8	},
	{"DarkGray",	9	},
};

XColor SCColor::xcolor[SC_COLOR_NUMBER] = {};

/// function	SCColor
/// brief		constructor
SCColor::SCColor()
{
}

/// function	~SCColor
/// brief		deconstructor
SCColor::~SCColor()
{
}

/// function	GetColor
/// function	GetColor
/// brief		get color with color name in X11
///
/// param		name	color name
/// return		Xcolor struct
XColor SCColor::GetColor(string name)
{
	return xcolor[color_map[name]];
}

/// function	AllocColor
/// brief		alloc color for eHMI
///
/// param		none
/// return		none
void SCColor::AllocColor(void)
{
	XColor real;
	map<string, int>::iterator it;
	Display* disp = SCDrawContext::GetDisplay();
	Colormap cmap = DefaultColormap(disp, 0);
	for (it = color_map.begin(); it != color_map.end(); ++it) {
		const char* name = (it->first).c_str();
		XColor* near = &xcolor[it->second];
		XAllocNamedColor(disp, cmap, name, near, &real);
		//Trace("%s is registered. (real color value = %x)\n", name, real);
	}
}

/// function	IsTransparent
/// brief		judge if the color is transparent
///
/// param		xcolor	the color for judgement
/// return		none
bool SCColor::IsTransparent(XColor xcolor)
{
	if (xcolor.red   == 0xD2D2 &&
		xcolor.green == 0x6969 &&
		xcolor.blue  == 0x1E1E) {
		return true;
	} else {
		return false;
	}
}

