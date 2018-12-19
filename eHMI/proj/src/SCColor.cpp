///
/// file		SCColor.cpp
/// brief		HMI color class source file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/12/19
///
#include "SCColor.h"

static std::unordered_map<int, string> colorMap = {
	{0,		"White"},
	{1, 	"Black"},
	{2, 	"Red"},
	{3,		"Yellow"},
	{4, 	"Blue"},
	{5, 	"Green"},
	{6, 	"Gray"},
	{7, 	"LightGray"},
	{8, 	"DarkGray"},
};

/// function	SCColor
/// brief		constructor
SCColor::SCColor(Display* display) :
disp(display),
cmap(DefaultColormap(display, 0))
{
}

/// function	~SCColor
/// brief		deconstructor
SCColor::~SCColor()
{
}

/// function	AllocColor
/// brief		alloc color for eHMI
///
/// param		none
/// return		none
void SCColor::AllocColor(void)
{
	XColor true_color;
	for (int i = 0; i < colorMap.size(); ++i) {
		XAllocNamedColor(disp, camp, colorMap.at(i).c_str(), &xcolor[i], &true_color);
	}
}

/// function	GetColor
/// brief		get color with color name in X11
///
/// param		p_color		color name
/// return		Xcolor struct
XColor SCColor::GetColor(char* p_color) {
	return xcolor[SC_COLOR_NUMBER];
}
