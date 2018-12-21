///
/// file		SCBase.h
/// brief		HMI draw base class define head file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/12/20
///

#ifndef __SCL_BASE_H__
#define __SCL_BASE_H__

///
/// Coordinate class
///
class SCPoint {
public :
	SCPoint() : x(0), y(0){};
	SCPoint(int xIn, int yIn) : x(xIn), y(yIn){};
	SCPoint operator =(const SCPoint& opt){
		x = opt.x;
		y = opt.y;
		return opt;
	};
	bool operator != (const SCPoint &opt) const {
		return ((x != opt.x) || (y != opt.y));
	};
	bool operator == (const SCPoint &opt) const {
		return ((x == opt.x) && (y == opt.y));
	};
	SCPoint operator +(const SCPoint &opt) const {
		SCPoint pos(x + opt.x, y + opt.y);
		return pos;
	};

public :
	int x;
	int y;
};

class SCRect {
public :
	SCRect(): x(0),	y(0), width(0),	height(0){};
	SCRect(unsigned short inX, unsigned short inY, short inWidth, unsigned short inHeight):
		x(inX),
		y(inY),
		width(inWidth),
		height(inHeight){
	};
	
	void Set(unsigned short inX,
			unsigned short inY,
			unsigned short inWidth,
			unsigned short inHeight) {
		x = inX;
		y = inY;
		width = inWidth;
		height = inHeight;
	};
	bool Contains(const SCPoint& point) const {
		if( (point.x >= x) && (point.x < x + width) &&
			(point.y >= y) && (point.y < y + height) ) {
			return(true);
		} else {
			return(false);
		}
	};
	bool Contains(unsigned short tx, unsigned short ty) const {
		if( (tx >= x) && (tx < x + width) &&
			(ty >= y) && (ty < y + height) ) {
			return(true);
		} else {
			return(false);
		}
	};
	bool Contains(const SCRect& rect) const {
		if( (rect.x >= x) && (rect.x + rect.width <= x + width) &&
			(rect.y >= y) && (rect.y + rect.height <= y + height) ) {
			return(true);
		} else {
			return(false);
		}
	};
	bool Overlap(const SCRect& rect) const {
		if( (rect.x < x + width) && (rect.y < y + height) &&
			(rect.x + width > x) && (rect.y + rect.height > y) ) {
			return(true);
		} else {
			return(false);
		}
	};
	SCRect operator =(const SCRect& rect){
		x		= rect.x;
		y 		= rect.y;
		width	= rect.width;
		height	= rect.height;
		return rect;
	};
	bool operator != (const SCRect& rect) const {
		if( (rect.x != x) || (rect.y != y) ||
			(rect.width != width) || (rect.height != height) ) {
			return(true);
		} else {
			return(false);
		}
	};
	bool operator == (const SCRect& rect) const {
		if( (rect.x == x) && (rect.y == y) &&
			(rect.width == width) && (rect.height == height) ) {
			return(true);
		} else {
			return(false);
		}
	};
	unsigned short Right(void) const {return(x + width - 1);};
	unsigned short Bottom(void) const {return(y + height - 1);};

public :
	unsigned short x;
	unsigned short y;
	unsigned short width;
	unsigned short height;
};

#endif // __SCL_BASE_H__

