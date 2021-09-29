/**********************************************************************
 * Copyright (c) 2018 - 2021 by Wang Yu
 * All rights reserved
 *
 * Filename:  assert_param.c
 * Project:   Minimum RTOS platform
 * Date:      2021/9/10
 * Author:    Wang Yu
 *
 **********************************************************************/
#include <stdint.h>
#include <stdio.h>

#ifdef  DEBUG
void assert_failed(uint8_t *file, uint32_t line)
{
	printf("Wrong parameters value: file %s on line %d.\r\n", file, (int)line);
	while(1);
}
#endif /* DEBUG */
