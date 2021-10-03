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
	static bool 	IsTransparent(uint32_t color){return color == SC_COLOR_TRANS;}

protected :
	static bool		drawRect(const uint32_t x,
							const uint32_t y,
							const uint32_t width,
							const uint32_t height,
							const uint32_t color);
	static bool		drawRect(const SCPoint pos,
							const uint32_t width,
							const uint32_t height,
							const uint32_t color);
	static bool		drawASCII(const uint32_t x,
							const uint32_t y,
							const char ascii,
							const uint32_t foreColor,
							const uint32_t back_color,
							const uint32_t font);
	static bool		drawASCII(const SCPoint pos,
							const char ascii,
							const uint32_t foreColor,
							const uint32_t back_color,
							const uint32_t font);
};

#endif // __SCL_DRAW_CONTEXT_H__
