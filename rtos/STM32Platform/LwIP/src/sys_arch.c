#include "lwip/debug.h"
#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/mem.h"
#include "lwip/stats.h"

#if !NO_SYS

#if defined(LWIP_PROVIDE_ERRNO)
int errno;
#endif

#define WAIT_FOREVER     0xFFFFFFFF

// System
static int inHandlerMode(void)
{
	return __get_IPSR() != 0;
}

static uint32_t sysTick(void)
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
	if (lwip_stats.sys.mbox.max < lwip_stats.sys.mbox.used) {
		lwip_stats.sys.mbox.max = lwip_stats.sys.mbox.used;
	}
#endif
	if (*mbox == NULL) return ERR_MEM;

	return ERR_OK;
}

void sys_mbox_free(sys_mbox_t *mbox)
{
	LWIP_ASSERT("Thread API (sys_mbox_free) is called in handler mode!", !inHandlerMode());
	if (uxQueueMessagesWaiting(*mbox)) {
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
	//while(osMessagePut(*mbox, (uint32_t)data, WAIT_FOREVER) != osOK);
	LWIP_ASSERT("Thread API (sys_mbox_post) is called in handler mode!", !inHandlerMode());

	portBASE_TYPE taskWoken = pdFALSE;
	TickType_t ticks;

	ticks = WAIT_FOREVER / portTICK_PERIOD_MS;
	if (ticks == 0) ticks = 1;

	do {
		taskWoken = xQueueSend(*mbox, &data, ticks);
	}
	while(taskWoken != pdTRUE);
}

err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
	LWIP_ASSERT("Thread API (sys_mbox_trypost) is called in handler mode!", !inHandlerMode());

	err_t result;
	//if (osMessagePut(*mbox, (uint32_t)msg, 0) == osOK) {
	if (xQueueSend(*mbox, &msg, 0) == pdTRUE) {
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
	//return sys_mbox_trypost(mbox, msg);
	err_t result;

	LWIP_ASSERT("Handler API (sys_mbox_trypost) is called in thread mode!", inHandlerMode());

	portBASE_TYPE taskWoken = pdFALSE;

	if (xQueueSendFromISR(*mbox, &msg, &taskWoken) == pdTRUE) {
		result = ERR_OK;
	}
	else {
		result = ERR_MEM;
	}
	portEND_SWITCHING_ISR(taskWoken);

	return result;
}

u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
	portBASE_TYPE	taskWoken;
	//osEvent			event;
	uint32_t		start = sysTick();
	TickType_t		ticks = 0;

	if (timeout == WAIT_FOREVER) {
		ticks = portMAX_DELAY;
	}
	else if (timeout != 0) {
		ticks = timeout / portTICK_PERIOD_MS;
		if (ticks == 0) ticks = 1;
	}

	portBASE_TYPE result = (inHandlerMode()) ?
							xQueueReceiveFromISR(*mbox, &(*msg), &taskWoken) :
							xQueueReceive(*mbox, &(*msg), ticks);

	if (timeout != 0 && result != pdTRUE) {
		return SYS_ARCH_TIMEOUT;
	}
	else {
		//*msg = (void *)event.value.v;
		return (sysTick() - start);
	}
}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
	//osEvent event;
	portBASE_TYPE taskWoken;
	portBASE_TYPE result = (inHandlerMode()) ?
							xQueueReceiveFromISR(*mbox, &(*msg), &taskWoken) :
							xQueueReceive(*mbox, &(*msg), 0);

	if (result != pdTRUE) {
		return SYS_MBOX_EMPTY;
	}
	else {
		//*msg = (void *)event.value.v;
		return ERR_OK;
	}
}

int sys_mbox_valid(sys_mbox_t *mbox)
{
	return (*mbox != SYS_MBOX_NULL);
	//if (*mbox == SYS_MBOX_NULL)
	//	return 0;
	//else
	//	return 1;
}

void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
	*mbox = SYS_MBOX_NULL;
}

// Semaphore
err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
	portBASE_TYPE taskWoken;
	//osSemaphoreDef(SEM);
	//*sem = osSemaphoreCreate (osSemaphore(SEM), 1);
	vSemaphoreCreateBinary(*sem);

	if (*sem == NULL) {
#if SYS_STATS
		++lwip_stats.sys.sem.err;
#endif /* SYS_STATS */
		return ERR_MEM;
	}

	if (count == 0) { // Means it can't be taken
		//osSemaphoreWait(*sem, 0);
		if (inHandlerMode()) {
			xSemaphoreTakeFromISR(*sem, &taskWoken);
			portEND_SWITCHING_ISR(taskWoken);
		}
		else {
			xSemaphoreTake(*sem, 0);
		}
	}

#if SYS_STATS
	++lwip_stats.sys.sem.used;
	if (lwip_stats.sys.sem.max < lwip_stats.sys.sem.used) {
		lwip_stats.sys.sem.max = lwip_stats.sys.sem.used;
	}
#endif

	return ERR_OK;
}

u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
	TickType_t		ticks = 0;
	portBASE_TYPE	result;
	portBASE_TYPE	taskWoken = pdFALSE;
	uint32_t		start = sysTick();

	if (timeout == 0) {
		ticks = portMAX_DELAY;
		//while(osSemaphoreWait (*sem, WAIT_FOREVER) != osOK);
		do {
			result = (inHandlerMode()) ?
						xSemaphoreTakeFromISR(*sem, &taskWoken) :
						xSemaphoreTake(*sem, ticks);
		} while (result != pdTRUE);
		if (inHandlerMode()) portEND_SWITCHING_ISR(taskWoken);
		return (sysTick() - start);
	}
	else {
		ticks = timeout / portTICK_PERIOD_MS;
		if (ticks == 0) ticks = 1;
		result = (inHandlerMode()) ?
					xSemaphoreTakeFromISR(*sem, &taskWoken) :
					xSemaphoreTake(*sem, ticks);
		if (result == pdTRUE) {
			if (inHandlerMode()) portEND_SWITCHING_ISR(taskWoken);
			return (sysTick() - start);
		}
		else {
			return SYS_ARCH_TIMEOUT;
		}
	}
}

void sys_sem_signal(sys_sem_t *sem)
{
	//osSemaphoreRelease(*sem);
	portBASE_TYPE taskWoken = pdFALSE;

	if (inHandlerMode()) {
		if (xSemaphoreGiveFromISR(*sem, &taskWoken) != pdTRUE) return;
		portEND_SWITCHING_ISR(taskWoken);
	}
	else {
		xSemaphoreGive(*sem);
	}
}

void sys_sem_free(sys_sem_t *sem)
{
	LWIP_ASSERT("Thread API (sys_mbox_trypost) is called in handler mode!", !inHandlerMode());

#if SYS_STATS
	--lwip_stats.sys.sem.used;
#endif

	//osSemaphoreDelete(*sem);
	vSemaphoreDelete(*sem);
}

int sys_sem_valid(sys_sem_t *sem)
{
	return (*sem != SYS_SEM_NULL);
	//if (*sem == SYS_SEM_NULL)
	//	return 0;
	//else
	//	return 1;
}

void sys_sem_set_invalid(sys_sem_t *sem)
{
	*sem = SYS_SEM_NULL;
}

SemaphoreHandle_t lwip_sys_mutex;
//osMutexDef(lwip_sys_mutex);

// Initialize sys arch
void sys_init(void)
{
	//lwip_sys_mutex = osMutexCreate(osMutex(lwip_sys_mutex));
	lwip_sys_mutex = xSemaphoreCreateMutex();
}

// Mutexes
#if LWIP_COMPAT_MUTEX == 0

err_t sys_mutex_new(sys_mutex_t *mutex) {
	//osMutexDef(MUTEX);
	//*mutex = osMutexCreate(osMutex(MUTEX));
	*mutex = xSemaphoreCreateMutex();

	if (*mutex == NULL)
	{
#if SYS_STATS
		++lwip_stats.sys.mutex.err;
#endif
		return ERR_MEM;
	}

#if SYS_STATS
	++lwip_stats.sys.mutex.used;
	if (lwip_stats.sys.mutex.max < lwip_stats.sys.mutex.used) {
		lwip_stats.sys.mutex.max = lwip_stats.sys.mutex.used;
	}
#endif
	return ERR_OK;
}

void sys_mutex_free(sys_mutex_t *mutex)
{
	LWIP_ASSERT("Thread API (sys_mbox_trypost) is called in handler mode!", !inHandlerMode());

#if SYS_STATS
	--lwip_stats.sys.mutex.used;
#endif

	//osMutexDelete(*mutex);
	vQueueDelete(*mutex);
}

void sys_mutex_lock(sys_mutex_t *mutex)
{
	//osMutexWait(*mutex, WAIT_FOREVER);
	LWIP_ASSERT("Mutex is null!", *mutex != NULL);

	TickType_t ticks		= portMAX_DELAY;
	portBASE_TYPE taskWoken	= pdFALSE;

	if (inHandlerMode()) {
		if (xSemaphoreTakeFromISR(*mutex, &taskWoken) != pdTRUE) return;
		portEND_SWITCHING_ISR(taskWoken);
	}
	else {
		xSemaphoreTake(*mutex, ticks);
	}
}

void sys_mutex_unlock(sys_mutex_t *mutex)
{
	//osMutexRelease(*mutex);
	portBASE_TYPE taskWoken = pdFALSE;

	if (inHandlerMode()) {
		if (xSemaphoreGiveFromISR(*mutex, &taskWoken) != pdTRUE) return;
		portEND_SWITCHING_ISR(taskWoken);
	}
	else {
		xSemaphoreGive(*mutex);
	}
}

#endif // LWIP_COMPAT_MUTEX

// Thread
sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread , void *arg, int stacksize, int prio)
{
	//const osThreadDef_t os_thread_def = { (char *)name, (os_pthread)thread, (osPriority)prio, 0, stacksize};
	//return osThreadCreate(&os_thread_def, arg);
	sys_thread_t handle;
	xTaskCreate((TaskFunction_t)thread, (const portCHAR *)name, stacksize,
				arg, (UBaseType_t)(3 + prio), (TaskHandle_t*)(&handle));
	return handle;
}

sys_prot_t sys_arch_protect(void)
{
	//osMutexWait(lwip_sys_mutex, WAIT_FOREVER);
	TickType_t ticks		= portMAX_DELAY;
	portBASE_TYPE taskWoken	= pdFALSE;

	if (inHandlerMode()) {
		if (xSemaphoreTakeFromISR(lwip_sys_mutex, &taskWoken) != pdTRUE) return (sys_prot_t)0;
		portEND_SWITCHING_ISR(taskWoken);
	}
	else {
		xSemaphoreTake(lwip_sys_mutex, ticks);
	}

	return (sys_prot_t)1;
}

void sys_arch_unprotect(sys_prot_t pval)
{
	( void ) pval;
	//osMutexRelease(lwip_sys_mutex);
	portBASE_TYPE taskWoken	= pdFALSE;

	if (inHandlerMode()) {
		if (xSemaphoreGiveFromISR(lwip_sys_mutex, &taskWoken) != pdTRUE) return;
		portEND_SWITCHING_ISR(taskWoken);
	}
	else {
		xSemaphoreGive(lwip_sys_mutex);
	}
}

#endif // !NO_SYS
