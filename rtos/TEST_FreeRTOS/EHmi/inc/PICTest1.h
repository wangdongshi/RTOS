/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename:  PICTest.h
 * Project:   Minimum RTOS platform
 * Date:      2018/12/20
 * Author:    WangYu
 *
 **********************************************************************/
#ifndef __PIC_TEST1_H__
#define __PIC_TEST1_H__

#include "SCBoard.h"

class PICTest1 : public SCBoard {
public :
	PICTest1(const SCRect& area, const short id = -1); // -1: illegal ID
	virtual ~PICTest1();

	void InitScreen(void);
};

#endif // __PIC_TEST1_H__
