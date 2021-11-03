#ifndef __SYS_ARCH_H__
#define __SYS_ARCH_H__

#include "lwip/opt.h"

#if (NO_SYS != 0)
#error "NO_SYS need to be set to 0 to use threaded API"
#endif

#include "cmsis_os.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define SYS_MBOX_NULL		(QueueHandle_t)0
#define SYS_SEM_NULL		(SemaphoreHandle_t)0

typedef enum  {
	osOK                    =     0,       ///< function completed; no error or event occurred.
	osEventSignal           =  0x08,       ///< function completed; signal event occurred.
	osEventMessage          =  0x10,       ///< function completed; message event occurred.
	osEventMail             =  0x20,       ///< function completed; mail event occurred.
	osEventTimeout          =  0x40,       ///< function completed; timeout occurred.
	osErrorParameter        =  0x80,       ///< parameter error: a mandatory parameter was missing or specified an incorrect object.
	osErrorResource         =  0x81,       ///< resource not available: a specified resource was not available.
	osErrorTimeoutResource  =  0xC1,       ///< resource not available within given time: a specified resource was not available within the timeout period.
	osErrorISR              =  0x82,       ///< not allowed in ISR context: the function cannot be called from interrupt service routines.
	osErrorISRRecursive     =  0x83,       ///< function called multiple times from ISR with same object.
	osErrorPriority         =  0x84,       ///< system cannot determine priority or thread has illegal priority.
	osErrorNoMemory         =  0x85,       ///< system is out of memory: it was impossible to allocate or reserve memory for the operation.
	osErrorValue            =  0x86,       ///< value of a parameter is out of range.
	osErrorOS               =  0xFF,       ///< unspecified RTOS error: run-time error but no other error message fits.
	os_status_reserved      =  0x7FFFFFFF  ///< prevent from enum down-size compiler optimization.
} osStatus;

typedef SemaphoreHandle_t	sys_sem_t;
typedef SemaphoreHandle_t	sys_mutex_t;
typedef QueueHandle_t		sys_mbox_t;
typedef TaskHandle_t		sys_thread_t;

#ifdef  __cplusplus
}
#endif

#endif // __SYS_ARCH_H__

