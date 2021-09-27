/**********************************************************************
 * Copyright (c) 2018 - 2021 by Wang Yu
 * All rights reserved
 *
 * Filename:  font06x08.h
 * Project:   Minimum RTOS platform
 * Date:      2021/9/25
 * Author:    Wang Yu
 *
 **********************************************************************/
#ifndef	_LCD_FONT06X08_H_
#define	_LCD_FONT06X08_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct _FONT_06X08 {
	uint8_t	nLines;			// font data lines (vertical)
	uint8_t	nByte;			// font data bytes
	uint8_t	nBit;			// effective bit (from first byte)
	uint8_t	fData[8][1];	// font data
} FONT_06X08;

typedef FONT_06X08 FONT_06X08_TBL[256];

#ifdef __cplusplus
}
#endif

#endif
