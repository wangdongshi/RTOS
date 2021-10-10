/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : EHmiAlloc.cpp
 * Project  : Minimum RTOS platform
 * Date     : 2021/9/5
 * Author   : WangYu
 *
 **********************************************************************/
#include "EHmiAlloc.h"

void* EHmiAlloc::operator new(size_t size)
{
	return pvPortMalloc(size);
}

void* EHmiAlloc::operator new[](size_t size)
{
	return pvPortMalloc(size);
}

void EHmiAlloc::operator delete(void * ptr)
{
	vPortFree (ptr);
}

void EHmiAlloc::operator delete[](void * ptr)
{
	vPortFree (ptr);
}