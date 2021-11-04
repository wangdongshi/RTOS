#ifndef __SYS_ARCH_H__
#define __SYS_ARCH_H__

#include "lwip/opt.h"

#if (NO_SYS != 0)
#error "NO_SYS need to be set to 0 to use threaded API"
#endif

#include "FreeRTOS.h"
#include "semphr.h"
//#include "cmsis_os.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define SYS_MBOX_NULL		(QueueHandle_t)0
#define SYS_SEM_NULL		(SemaphoreHandle_t)0

typedef SemaphoreHandle_t	sys_sem_t;
typedef SemaphoreHandle_t	sys_mutex_t;
typedef QueueHandle_t		sys_mbox_t;
typedef TaskHandle_t		sys_thread_t;

#ifdef  __cplusplus
}
#endif

#endif // __SYS_ARCH_H__

