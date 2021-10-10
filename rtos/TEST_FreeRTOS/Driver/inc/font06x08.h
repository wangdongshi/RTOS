/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : font06x08.h
 * Project  : Minimum RTOS platform
 * Date     : 2021/9/25
 * Author   : WangYu
 *
 **********************************************************************/
#ifndef	__FONT06X08_H__
#define	__FONT06X08_H__

#include "types.h"

typedef struct {
    uint8_t		fontWidth;
    uint8_t		fontHeight;
} FONT_06X08_HEADER;

typedef struct {
	uint8_t		width;
    uint8_t		data[6][1];
} FONT_06X08_CONTENT;

typedef struct {
	FONT_06X08_HEADER	head;
	FONT_06X08_CONTENT	cdef[255];
} FONT_06X08_TABLE;

typedef union {
	FONT_06X08_TABLE	st;
	uint8_t 			array[2 + 7 * 255];
} FONT_06X08;

extern const FONT_06X08 sFont;

#endif // __FONT06X08_H__
