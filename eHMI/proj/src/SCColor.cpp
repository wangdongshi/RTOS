///
/// file		SCColor.cpp
/// brief		HMI color class source file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/12/19
///
#include "SCColor.h"
#include "SCDrawContext.h"

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
SCColor::SCColor()
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
/// param		name	color name
/// return		Xcolor struct
XColor SCColor::GetColor(string name)
{
	return xcolor[color_map[name]];
}

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
	}
}

