/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : PICTest2.h
 * Project  : Minimum RTOS platform
 * Date     : 2018/11/26
 * Author   : WangYu
 *
 **********************************************************************/
#ifndef __PIC_TEST2_H__
#define __PIC_TEST2_H__

#include "SCBoard.h"

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
