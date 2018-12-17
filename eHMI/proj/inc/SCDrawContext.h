///
/// file		SCDrawContext.h
/// brief		HMI draw context class define head file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/12/07
///

#ifndef __SCL_BASE_H__
#define __SCL_BASE_H__

#include <X11/Xlib.h>

const char font_type[3][100] = {
	"-misc-fixed-medium-r-normal--14-130-75-75-c-70-iso8859-1",
	"-*-helvetica-medium-r-normal--11-80-100-100-p-56-*",
	"-Adobe-Courier-Medium-R-Normal-*-*-180-75-*",
};

#define SC_FONT_LARGE	const_cast<char*>(font_type[0])
#define SC_FONT_MIDDLE	const_cast<char*>(font_type[1])
#define SC_FONT_SMALL	const_cast<char*>(font_type[2])

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
	SCDrawContext() {};
	SCDrawContext(Display* d, Window& w) : disp(d), win(w) {
		gc = XCreateGC(disp, win, 0, NULL);
	};
	virtual ~SCDrawContext() {
		XFreeGC(disp, gc);
	};

public:
	Display*		getDisplay(void) {return disp;};
	Window*			getWindow(void) {return &win;};
	GC*				getGC(void) {return &gc;};

public: // change this to protected after create SCDrawCommand class
	void			drawRect(const SCPos pos,
							const unsigned int width,
							const unsigned int height,
							const unsigned int color) {
			//XSetForeground(disp, gc, BlackPixel(disp, win));
			Trace("stepC\n");
			XFillRectangle(disp, win, gc, pos.x, pos.y, width, height);
	};
	void			drawChar(const SCPos pos,
							const unsigned int back_color,
							const unsigned int fore_color,
							char* font_name,
							char ascii) {
			Trace("stepX\n");
			Font font = XLoadFont(disp, font_name);
			Trace("step0\n");
			if (!font) Trace("XLoadFont Error!");
			XSetFont(disp, gc, font);
			Trace("step1\n");
			XTextItem text = {&ascii, 1, 0, font};
			Trace("step2\n");
			XDrawText(disp, win, gc, pos.x, pos.y, &text, 1);
			Trace("step3\n");
			XUnloadFont(disp, font);
			Trace("step4\n");
			//XFreeFont(disp, &font);
	};

private:
	Display*		disp;
	Window			win;
	GC				gc;
};

#endif // __SCL_BASE_H__

