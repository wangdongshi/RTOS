/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : ft5336.h
 * Project  : Minimum RTOS platform
 * Date     : 2021/9/20
 * Author   : WangYu
 *
 **********************************************************************/
#ifndef __FT5336_H__
#define __FT5336_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

void writeFT5336DeviceMode(uint8_t data);
void enableFT5336Int(void);
void disableFT5336Int(void);

uint8_t readFT5336ChipID(void);
uint8_t readFT5336DeviceMode(void);
uint8_t readFT5336GMode(void);
uint8_t readFT5336GestureID(void);
uint8_t readFT5336PointNum(void);
uint8_t readFT5336EventFlag(void);
uint16_t readFT5336PointX(void);
uint16_t readFT5336PointY(void);

#ifdef __cplusplus
}
#endif

#endif // __FT5336_H__
