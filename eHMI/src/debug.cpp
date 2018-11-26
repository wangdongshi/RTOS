#include <stdio.h>
#include <stdarg.h>
#include "debug.h"

#ifdef __cplusplus
extern "C" {
#endif

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
