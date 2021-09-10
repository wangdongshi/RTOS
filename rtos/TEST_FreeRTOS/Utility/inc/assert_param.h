/**********************************************************************
 * Copyright (c) 2018 - 2021 by Wang Yu
 * All rights reserved
 *
 * Filename:  assert_param.h
 * Project:   Minimum RTOS platform
 * Date:      2021/9/5
 * Author:    Wang Yu
 *
 **********************************************************************/
#ifndef ASSERT_PARAM_H
#define ASSERT_PARAM_H

#ifdef  USE_FULL_ASSERT
  #define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0U)
#endif /* USE_FULL_ASSERT */

#endif
