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

using namespace std;

map<string, int> SCColor::color_map = {
	{"White",		0	},
	{"Black",		1	},
	{"Red",			2	},
	{"Yellow",		3	},
	{"Blue",		4	},
	{"Green",		5	},
	{"Gray",		6	},
	{"LightGray",	7	},
	{"DarkGray",	8	},
};

XColor SCColor::xcolor[SC_COLOR_NUMBER] = {};

/// function	SCColor
/// brief		constructor
SCColor::SCColor(Display* display) :
disp(display),
cmap(&DefaultColormap(display, 0))
{
}

/// function	~SCColor
/// brief		deconstructor
SCColor::~SCColor()
{
}

/// function	AllocColor
/// function	GetColor
/// brief		get color with color name in X11
///
/// param		p_color		color name
/// return		Xcolor struct
XColor SCColor::GetColor(string color_name) {
	return xcolor[color_map[color_name]];
}

/// brief		alloc color for eHMI
///
/// param		none
/// return		none
void SCColor::AllocColor(void)
{
	XColor true_color;
	map<string, int>::iterator it;
	for (it = color_map.begin(); it != color_map.end(); ++it) {
		XAllocNamedColor(disp, *cmap, (it->first).c_str(), &xcolor[it->second], &true_color);
	}
}

