///
/// file		SCDrawContext.h
/// brief		HMI draw context class define head file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/12/07
///

#ifndef __SCL_DRAW_CONTEXT_H__
#define __SCL_DRAW_CONTEXT_H__

#include <X11/Xlib.h>

#define SC_FONT_LARGE	const_cast<char*>(SCDrawContext::font_type[0])
#define SC_FONT_MIDDLE	const_cast<char*>(SCDrawContext::font_type[1])
#define SC_FONT_SMALL	const_cast<char*>(SCDrawContext::font_type[2])

///
/// class : SCPos
/// Coordinate class
///
class SCPos {
public :
	SCPos() : x(0), y(0){};
	SCPos(int xIn, int yIn) : x(xIn), y(yIn){};
	SCPos operator =(const SCPos& opt){x = opt.x; y = opt.y; return opt;};
	bool operator != (const SCPos &opt) const {
			return ((x != opt.x) || (y != opt.y));
	};
	bool operator == (const SCPos &opt) const {
			return ((x == opt.x) && (y == opt.y));
	};
	SCPos operator +(const SCPos &opt) const {
			SCPos pos(x + opt.x, y + opt.y);
			return pos;
	};

public :
	int x;
	int y;
};

class SCDrawContext {
public:
	SCDrawContext();
	SCDrawContext(Display* d, Window& w);
	virtual ~SCDrawContext();

public:
	Display*		getDisplay(void) {return disp;};
	Window*			getWindow(void) {return &win;};
	GC*				getGC(void) {return &gc;};

public: // change this to protected after create SCDrawCommand class
	void			drawRect(const unsigned int x,
							const unsigned int y,
							const unsigned int width,
							const unsigned int height,
							const unsigned int color);
	void			drawRect(const SCPos pos,
							const unsigned int width,
							const unsigned int height,
							const unsigned int color);
	void			drawASCII(const unsigned int x,
							const unsigned int y,
							const char ascii,
							const unsigned int fore_color,
							const unsigned int back_color,
							char* font_name);
	void			drawASCII(const SCPos pos,
							const char ascii,
							const unsigned int fore_color,
							const unsigned int back_color,
							char* font_name);

public:
	static const char font_type[3][70];
	
private:
	Display*		disp;
	Window			win;
	GC				gc;
};

#endif // __SCL_DRAW_CONTEXT_H__

