///
/// file		debug.cpp
/// brief		the trace function definition file
///
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/11/26
///
#include <stdio.h>
#include <stdarg.h>
#include "debug.h"

#ifdef __cplusplus
extern "C" {
#endif

/// function	hmiMain
/// brief		eHMI main function
/// param		format	: string of format
/// param		...		: var parameter list
/// return		none
#ifdef DEBUG
void trace(const char *format, ...)
{
    va_list arg;

    va_start(arg, format);				   /* get var parameter list*/
    fflush(stdout);						   /* force to flush output buffer */
    vfprintf(stderr, format, arg); /* print message to stand error */
    va_end(arg);						   /* finish var parameter list */
}
#else
void trace(const char *format, ...)
{
}
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
