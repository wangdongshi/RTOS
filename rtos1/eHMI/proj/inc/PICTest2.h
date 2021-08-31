///
/// file		PICTest2.h
/// brief		test2 screen class define head file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2019/01/10
///

#ifndef __PIC_TEST2_H__
#define __PIC_TEST2_H__

#include "SCBoard.h"

///
/// class : PICTest2
/// Test1 screen class
///
class PICTest2 : public SCBoard {
public :
	PICTest2(const SCRect& area, const short id = -1); // -1: illegal ID
	virtual ~PICTest2();

	void InitScreen(void);
	void ButtonPushed(const int id);

private :
	static const SCRect		area_top_label;
	static const SCRect		area_button;
	static const SCRect		area_time_label;
};

#endif // __PIC_TEST2_H__
