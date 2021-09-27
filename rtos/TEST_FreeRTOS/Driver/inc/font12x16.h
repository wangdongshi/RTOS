/**********************************************************************
 * Copyright (c) 2018 - 2021 by Wang Yu
 * All rights reserved
 *
 * Filename:  font12x16.h
 * Project:   Minimum RTOS platform
 * Date:      2021/9/25
 * Author:    Wang Yu
 *
 **********************************************************************/
#ifndef	_LCD_FONT12X16_H_
#define	_LCD_FONT12X16_H_

#include <stdint.h>

typedef struct _FONT_12X16 {
	uint8_t	lines;			// font data lines (vertical)
	uint8_t	bytes;			// font data bytes
	uint8_t	width;			// font data effective width (bit number from first byte)
	uint8_t	fData[16][2];	// font data
} FONT_12X16;

typedef FONT_12X16 FONT_12X16_TBL[256];

const FONT_12X16_TBL mFont;

#endif
