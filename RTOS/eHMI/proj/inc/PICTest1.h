///
/// file		PICTest1.h
/// brief		test1 screen class define head file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/12/24
///

#ifndef __PIC_TEST1_H__
#define __PIC_TEST1_H__

#include "SCBoard.h"

///
/// class : PICTest1
/// Test1 screen class
///
class PICTest1 : public SCBoard {
public :
	PICTest1(const SCRect& area, const short id = -1); // -1: illegal ID
	virtual ~PICTest1();

	void InitScreen(void);
	void ButtonPushed(const int id);

private :
	static const SCRect		area_label;
	static const SCRect		area_button;
};

#endif // __PIC_TEST1_H__
