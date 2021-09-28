/**********************************************************************
 * Copyright (c) 2018 - 2021 by Wang Yu
 * All rights reserved
 *
 * Filename:  font12X16.h
 * Project:   Minimum RTOS platform
 * Date:      2021/9/25
 * Author:    Wang Yu
 *
 **********************************************************************/
#ifndef	_FONT12X16_H_
#define	_FONT12X16_H_

#include <stdint.h>

typedef struct {
    uint8_t		fontWidth;
    uint8_t		fontHeight;
} FONT_12X16_HEADER;

typedef struct {
	uint8_t		width;
    uint8_t		data[12][2];
} FONT_12X16_CONTENT;

typedef struct {
	FONT_12X16_HEADER	head;
	FONT_12X16_CONTENT	cdef[128];
} FONT_12X16_TABLE;

typedef union {
	FONT_12X16_TABLE	st;
	uint8_t 			array[2 + 25 * 128];
} FONT_12X16;

extern const FONT_12X16 mFont;

#endif
