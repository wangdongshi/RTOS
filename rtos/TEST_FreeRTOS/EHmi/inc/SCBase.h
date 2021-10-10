/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : SCBase.h
 * Project  : Minimum RTOS platform
 * Date     : 2018/11/26
 * Author   : WangYu
 *
 **********************************************************************/
#ifndef __SCL_BASE_H__
#define __SCL_BASE_H__

#define SC_FONT_SMALL		FONT_SMALL
#define SC_FONT_MIDDLE		FONT_MIDDLE

#define SC_COLOR_TRANS		(0x00000000)
#define SC_COLOR_WHITE		(0xFFFFFFFF)
#define SC_COLOR_BLACK		(0xFF000000)
#define SC_COLOR_RED		(0xFFFF0000)
#define SC_COLOR_YELLOW		(0xFFFFFF00)
#define SC_COLOR_BLUE		(0xFF0000FF)
#define SC_COLOR_GREEN		(0xFF00FF00)
#define SC_COLOR_GRAY		(0xFFBEBEBE)
#define SC_COLOR_LIGHT_GRAY	(0xFFD3D3D3)
#define SC_COLOR_DARK_GRAY	(0xFFA9A9A9)
#define SC_COLOR_ROYAL_BLUE	(0xFF4169E1)

#define ARRANGE_LEFT		(0)
#define ARRANGE_MIDDLE		(1)
#define ARRANGE_RIGHT		(2)

// base window size
#define WINDOW_WIDTH		LCD_ACTIVE_WIDTH
#define WINDOW_HEIGHT		LCD_ACTIVE_HEIGHT

class SCPoint {
public :
	SCPoint() : x(0), y(0){}
	SCPoint(int xIn, int yIn) : x(xIn), y(yIn){}
	SCPoint operator =(const SCPoint& opt){
		x = opt.x;
		y = opt.y;
		return opt;
	}
	bool operator != (const SCPoint &opt) const {
		return ((x != opt.x) || (y != opt.y));
	};
	bool operator == (const SCPoint &opt) const {
		return ((x == opt.x) && (y == opt.y));
	}
	SCPoint operator +(const SCPoint &opt) const {
		SCPoint pos(x + opt.x, y + opt.y);
		return pos;
	}

public :
	int x;
	int y;
};

class SCRect {
public :
	SCRect(): x(0),	y(0), width(0),	height(0){}
	SCRect(unsigned short inX, unsigned short inY, short inWidth, unsigned short inHeight):
		x(inX),
		y(inY),
		width(inWidth),
		height(inHeight){
	}
	
	void Set(unsigned short inX,
			unsigned short inY,
			unsigned short inWidth,
			unsigned short inHeight) {
		x = inX;
		y = inY;
		width = inWidth;
		height = inHeight;
	}
	bool Contains(const SCPoint& point) const {
		if( (point.x >= x) && (point.x < x + width) &&
			(point.y >= y) && (point.y < y + height) ) {
			return(true);
		}
		else {
			return(false);
		}
	}
	bool Contains(unsigned short tx, unsigned short ty) const {
		if( (tx >= x) && (tx < x + width) &&
			(ty >= y) && (ty < y + height) ) {
			return(true);
		}
		else {
			return(false);
		}
	}
	bool Contains(const SCRect& rect) const {
		if( (rect.x >= x) && (rect.x + rect.width <= x + width) &&
			(rect.y >= y) && (rect.y + rect.height <= y + height) ) {
			return(true);
		}
		else {
			return(false);
		}
	}
	bool Overlap(const SCRect& rect) const {
		if( (rect.x < x + width) && (rect.y < y + height) &&
			(rect.x + width > x) && (rect.y + rect.height > y) ) {
			return(true);
		}
		else {
			return(false);
		}
	}
	SCRect operator =(const SCRect& rect){
		x		= rect.x;
		y 		= rect.y;
		width	= rect.width;
		height	= rect.height;
		return rect;
	}
	bool operator != (const SCRect& rect) const {
		if( (rect.x != x) || (rect.y != y) ||
			(rect.width != width) || (rect.height != height) ) {
			return(true);
		}
		else {
			return(false);
		}
	}
	bool operator == (const SCRect& rect) const {
		if( (rect.x == x) && (rect.y == y) &&
			(rect.width == width) && (rect.height == height) ) {
			return(true);
		}
		else {
			return(false);
		}
	}
	unsigned short Right(void) const {return(x + width - 1);}
	unsigned short Bottom(void) const {return(y + height - 1);}

public :
	unsigned short x;
	unsigned short y;
	unsigned short width;
	unsigned short height;
};

#endif // __SCL_BASE_H__

