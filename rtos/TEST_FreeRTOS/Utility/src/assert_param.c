/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : assert_param.c
 * Project  : Minimum RTOS platform
 * Date     : 2021/9/10
 * Author   : WangYu
 *
 **********************************************************************/
#include <stdio.h>

#ifdef DEBUG
void assert_failed(unsigned char* file, int line)
{
	printf("Wrong parameters value: file %s on line %d.\r\n", file, (int)line);
	while(1);
}
#endif /* DEBUG */
