/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename:  SCDrawContext.h
 * Project:   Minimum RTOS platform
 * Date:      2018/11/26
 * Author:    WangYu
 *
 **********************************************************************/
#ifndef __SCL_DRAW_CONTEXT_H__
#define __SCL_DRAW_CONTEXT_H__

#include "stm32f746g_disco.h"
#include "SCBase.h"

#define IsTrans				SCDrawContext::IsTransparent

class SCDrawContext {
public :
	SCDrawContext();
	virtual ~SCDrawContext();

	static bool 	Initialize(void);
	static bool 	IsTransparent(unsigned int color){return color == SC_COLOR_TRANS;}

protected :
	static bool		drawRect(const unsigned int x,
							const unsigned int y,
							const unsigned int width,
							const unsigned int height,
							const unsigned int color);
	static bool		drawRect(const SCPoint pos,
							const unsigned int width,
							const unsigned int height,
							const unsigned int color);
	static bool		drawASCII(const unsigned int x,
							const unsigned int y,
							const char ascii,
							const unsigned int fore_color,
							const unsigned int back_color,
							const unsigned int font);
	static bool		drawASCII(const SCPoint pos,
							const char ascii,
							const unsigned int fore_color,
							const unsigned int back_color,
							const unsigned int font);
};

#endif // __SCL_DRAW_CONTEXT_H__
