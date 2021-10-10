/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : assert_param.h
 * Project  : Minimum RTOS platform
 * Date     : 2021/9/5
 * Author   : WangYu
 *
 **********************************************************************/
#ifndef __ASSERT_PARAM_H__
#define __ASSERT_PARAM_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef  DEBUG
	#define assert_param(expr) ((expr) ? (void)0U : assert_failed((unsigned char*)__FILE__, __LINE__))
	// export function
	void assert_failed(unsigned char* file, int line);
#else
	#define assert_param(expr) ((void)0U)
#endif /* DEBUG */

#ifdef __cplusplus
}
#endif

#endif // __ASSERT_PARAM_H__
