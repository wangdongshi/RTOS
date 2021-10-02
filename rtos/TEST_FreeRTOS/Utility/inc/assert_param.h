/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename:  assert_param.h
 * Project:   Minimum RTOS platform
 * Date:      2021/9/5
 * Author:    WangYu
 *
 **********************************************************************/
#ifndef __ASSERT_PARAM_H__
#define __ASSERT_PARAM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifdef  DEBUG
  #define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0U)
#endif /* DEBUG */

#ifdef __cplusplus
}
#endif

#endif // __ASSERT_PARAM_H__
