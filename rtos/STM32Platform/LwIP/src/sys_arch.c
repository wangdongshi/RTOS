#include "lwip/debug.h"
#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/mem.h"
#include "lwip/stats.h"

#if !NO_SYS

#include "cmsis_os.h"

#if defined(LWIP_PROVIDE_ERRNO)
int errno;
#endif

#define osWaitForever     0xFFFFFFFF

static int inHandlerMode(void)
{
	return __get_IPSR() != 0;
}

static osStatus osMessagePut(QueueHandle_t queue_id, uint32_t info, uint32_t millisec)
{
	portBASE_TYPE taskWoken = pdFALSE;
	TickType_t ticks;

	ticks = millisec / portTICK_PERIOD_MS;
	if (ticks == 0) {
		ticks = 1;
	}

	if (inHandlerMode()) {
		if (xQueueSendFromISR(queue_id, &info, &taskWoken) != pdTRUE) {
			return osErrorOS;
		}
		portEND_SWITCHING_ISR(taskWoken);
	}
	else {
		if (xQueueSend(queue_id, &info, ticks) != pdTRUE) {
			return osErrorOS;
		}
	}

	return osOK;
}

static uint32_t osKernelSysTick(void)
{
	if (inHandlerMode()) {
		return xTaskGetTickCountFromISR();
	}
	else {
		return xTaskGetTickCount();
	}
}

// Mailbox
err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
	//osMessageQDef(QUEUE, size, void *);
	//*mbox = osMessageCreate(osMessageQ(QUEUE), NULL);
	*mbox = xQueueCreate(size, sizeof(void*));
#if SYS_STATS
	++lwip_stats.sys.mbox.used;
	if(lwip_stats.sys.mbox.max < lwip_stats.sys.mbox.used) {
		lwip_stats.sys.mbox.max = lwip_stats.sys.mbox.used;
	}
#endif
	if(*mbox == NULL) return ERR_MEM;

	return ERR_OK;
}

void sys_mbox_free(sys_mbox_t *mbox)
{
	LWIP_ASSERT("Delete mailbox in Handler mode!", !inHandlerMode());
	if(uxQueueMessagesWaiting(*mbox)) {
		// Line for breakpoint.
		// Should never break here!
		portNOP();
#if SYS_STATS
		lwip_stats.sys.mbox.err++;
#endif
	}

	vQueueDelete(*mbox);
#if SYS_STATS
	--lwip_stats.sys.mbox.used;
#endif
}

void sys_mbox_post(sys_mbox_t *mbox, void *data)
{
	while(osMessagePut(*mbox, (uint32_t)data, osWaitForever) != osOK);
}

err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
	err_t result;
	if(osMessagePut(*mbox, (uint32_t)msg, 0) == osOK) {
		result = ERR_OK;
	}
	else {
		// could not post, queue must be full
		result = ERR_MEM;

#if SYS_STATS
		lwip_stats.sys.mbox.err++;
#endif /* SYS_STATS */
	}

	return result;
}

err_t sys_mbox_trypost_fromisr(sys_mbox_t *mbox, void *msg)
{
	return sys_mbox_trypost(mbox, msg);
}

u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
	osEvent event;
	uint32_t starttime = osKernelSysTick();
	if(timeout != 0) {
		event = osMessageGet(*mbox, timeout);
		if(event.status == osEventMessage) {
			*msg = (void *)event.value.v;
			return (osKernelSysTick() - starttime);
		}
		else {
			return SYS_ARCH_TIMEOUT;
		}
	}
	else {
		event = osMessageGet (*mbox, osWaitForever);
		*msg = (void *)event.value.v;
		return (osKernelSysTick() - starttime);
	}
}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
  osEvent event;

  event = osMessageGet (*mbox, 0);

  if(event.status == osEventMessage)
  {
    *msg = (void *)event.value.v;
    return ERR_OK;
  }
  else
  {
    return SYS_MBOX_EMPTY;
  }
}

int sys_mbox_valid(sys_mbox_t *mbox)
{
  if (*mbox == SYS_MBOX_NULL)
    return 0;
  else
    return 1;
}

void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
  *mbox = SYS_MBOX_NULL;
}

// Creates a new semaphore. The "count" argument specifies
// the initial state of the semaphore.
err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
  osSemaphoreDef(SEM);
  *sem = osSemaphoreCreate (osSemaphore(SEM), 1);

  if(*sem == NULL)
  {
#if SYS_STATS
    ++lwip_stats.sys.sem.err;
#endif /* SYS_STATS */
    return ERR_MEM;
  }

  if(count == 0)	// Means it can't be taken
  {
    osSemaphoreWait(*sem, 0);
  }

#if SYS_STATS
  ++lwip_stats.sys.sem.used;
  if (lwip_stats.sys.sem.max < lwip_stats.sys.sem.used) {
    lwip_stats.sys.sem.max = lwip_stats.sys.sem.used;
  }
#endif /* SYS_STATS */

  return ERR_OK;
}

u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
  uint32_t starttime = osKernelSysTick();
  if(timeout != 0)
  {
    if(osSemaphoreWait (*sem, timeout) == osOK)
    {
      return (osKernelSysTick() - starttime);
    }
    else
    {
      return SYS_ARCH_TIMEOUT;
    }
  }
  else
  {
    while(osSemaphoreWait (*sem, osWaitForever) != osOK);
    return (osKernelSysTick() - starttime);
  }
}

// Signals a semaphore
void sys_sem_signal(sys_sem_t *sem)
{
  osSemaphoreRelease(*sem);
}

// Deallocates a semaphore
void sys_sem_free(sys_sem_t *sem)
{
#if SYS_STATS
  --lwip_stats.sys.sem.used;
#endif /* SYS_STATS */

  osSemaphoreDelete(*sem);
}

int sys_sem_valid(sys_sem_t *sem)
{
  if (*sem == SYS_SEM_NULL)
    return 0;
  else
    return 1;
}

void sys_sem_set_invalid(sys_sem_t *sem)
{
  *sem = SYS_SEM_NULL;
}

osMutexId lwip_sys_mutex;
osMutexDef(lwip_sys_mutex);

// Initialize sys arch
void sys_init(void)
{
  lwip_sys_mutex = osMutexCreate(osMutex(lwip_sys_mutex));
}

// Mutexes
#if LWIP_COMPAT_MUTEX == 0
// Create a new mutex
err_t sys_mutex_new(sys_mutex_t *mutex) {
  osMutexDef(MUTEX);
  *mutex = osMutexCreate(osMutex(MUTEX));

  if(*mutex == NULL)
  {
#if SYS_STATS
    ++lwip_stats.sys.mutex.err;
#endif /* SYS_STATS */
    return ERR_MEM;
  }

#if SYS_STATS
  ++lwip_stats.sys.mutex.used;
  if (lwip_stats.sys.mutex.max < lwip_stats.sys.mutex.used) {
    lwip_stats.sys.mutex.max = lwip_stats.sys.mutex.used;
  }
#endif /* SYS_STATS */
  return ERR_OK;
}

// Deallocate a mutex
void sys_mutex_free(sys_mutex_t *mutex)
{
#if SYS_STATS
      --lwip_stats.sys.mutex.used;
#endif /* SYS_STATS */

  osMutexDelete(*mutex);
}

// Lock a mutex
void sys_mutex_lock(sys_mutex_t *mutex)
{
  osMutexWait(*mutex, osWaitForever);
}

// Unlock a mutex
void sys_mutex_unlock(sys_mutex_t *mutex)
{
  osMutexRelease(*mutex);
}
#endif // LWIP_COMPAT_MUTEX

// Thread
sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread , void *arg, int stacksize, int prio)
{
  const osThreadDef_t os_thread_def = { (char *)name, (os_pthread)thread, (osPriority)prio, 0, stacksize};
  return osThreadCreate(&os_thread_def, arg);
}

sys_prot_t sys_arch_protect(void)
{
  osMutexWait(lwip_sys_mutex, osWaitForever);
  return (sys_prot_t)1;
}

void sys_arch_unprotect(sys_prot_t pval)
{
  ( void ) pval;
  osMutexRelease(lwip_sys_mutex);
}

#endif // !NO_SYS
