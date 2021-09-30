/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename:  EHmiAlloc.cpp
 * Project:   Minimum RTOS platform
 * Date:      2021/9/5
 * Author:    WangYu
 *
 **********************************************************************/
#ifndef __EHMIALLOC_H__
#define __EHMIALLOC_H__

//#include <new>
#include "platform.h"

class EHmiAlloc {
public:
	EHmiAlloc() {}

public:
	static void*	operator new(size_t size);
	static void*	operator new[](size_t size);
	static void		operator delete(void * ptr);
	static void		operator delete[](void * ptr);
};

#endif // __EHMIALLOC_H__
