///
/// file		SCCore.h
/// brief		HMI parts base class define head file
///
/// author		Wang.Yu
/// version 		00.01.00
/// date		2018/12/07
///

#ifndef __SCL_CORE_H__
#define __SCL_CORE_H__

///
/// class : SCPos
/// Coordinate class
///
class SCPos {
public :
	SCPos() : x(0), y(0){}
	SCPos(int xIn, int yIn) : x(xIn), y(yIn){}
	SCPos operator =(const SCPos& opt){x = opt.x; y = opt.y; return opt;}
	bool operator != (const SCPos &opt) const {
			return ((x != opt.x) || (y != opt.y));
	}
	bool operator == (const SCPos &opt) const {
			return ((x == opt.x) && (y == opt.y));
	}
	SCPos operator +(const SCPos &opt) const {
			return pos(x + opt.x, y + opt.y);
	}

public :
	int x;
	int y;
}


///
/// class : SCCore
/// HMI core class
///
class SCCore {
public :
    EHmiEvent();
    EHmiEvent(EHmiEventType ev);
    EHmiEvent(EHmiEventType ev, unsigned long lp);
	/*
    EHmiEventInfo(EHmiEventType ev, 
			unsigned int ip1,
			unsigned int ip2
			);
    EHmiEventInfo(EHmiEventType ev,
			unsigned short sp1,
			unsigned short sp2,
			unsigned short sp3,
			unsigned short sp4
			);
	*/
    ~EHmiEvent();
    EHmiEventType GetEvent() {return(type);}
    unsigned long GetULArg() {return(arg.lp);}
	void SetULArg(unsigned long ul) {arg.lp = ul;}
	
private :
    EHmiEventType	type;
    EHmiEventParam	arg;
};

#endif // __SCL_CORE_H__
