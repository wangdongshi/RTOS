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

SemaphoreHandle_t lwip_sys_mutex;

// System
static int inHandlerMode(void)
{
	uint32_t result;

	__asm volatile ("MRS %0, ipsr" : "=r" (result) );
	return(result);
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
	if (xQueueSend(*mbox, &msg, 0) == pdTRUE) {
		result = ERR_OK;
	}
	else {
		// could not post, queue must be full
		result = ERR_MEM;
#if SYS_STATS
		lwip_stats.sys.mbox.err++;
#endif
	}

	return result;
}

err_t sys_mbox_trypost_fromisr(sys_mbox_t *mbox, void *msg)
{
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
	uint32_t		start = sysTick();
	TickType_t		ticks = 0;
	portBASE_TYPE 	result;

	if (*mbox == NULL) {
		return SYS_ARCH_TIMEOUT;
	}

	if (timeout == WAIT_FOREVER || timeout == 0) {
		ticks = portMAX_DELAY;
	}
	else {
		ticks = timeout / portTICK_PERIOD_MS;
		if (ticks == 0) ticks = 1;
	}

	if (inHandlerMode()) {
		result = xQueueReceiveFromISR(*mbox, &(*msg), &taskWoken);
		portEND_SWITCHING_ISR(taskWoken);
	}
	else {
		result = xQueueReceive(*mbox, &(*msg), ticks);
	}

	if (timeout == 0 || result == pdTRUE) {
		return (sysTick() - start);
	}
	else {
		return SYS_ARCH_TIMEOUT;
	}
}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
	portBASE_TYPE taskWoken;
	portBASE_TYPE result = (inHandlerMode()) ?
							xQueueReceiveFromISR(*mbox, &(*msg), &taskWoken) :
							xQueueReceive(*mbox, &(*msg), 0);

	if (result != pdTRUE) {
		return SYS_MBOX_EMPTY;
	}
	else {
		return ERR_OK;
	}
}

int sys_mbox_valid(sys_mbox_t *mbox)
{
	return (*mbox != SYS_MBOX_NULL);
}

void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
	*mbox = SYS_MBOX_NULL;
}

// Semaphore
err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
	portBASE_TYPE taskWoken;
	vSemaphoreCreateBinary(*sem);

	if (*sem == NULL) {
#if SYS_STATS
		++lwip_stats.sys.sem.err;
#endif
		return ERR_MEM;
	}

	if (count == 0) { // Means it can't be taken
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
	vSemaphoreDelete(*sem);
}

int sys_sem_valid(sys_sem_t *sem)
{
	return (*sem != SYS_SEM_NULL);
}

void sys_sem_set_invalid(sys_sem_t *sem)
{
	*sem = SYS_SEM_NULL;
}

// Mutex
void sys_init(void)
{
	lwip_sys_mutex = xSemaphoreCreateMutex();
}

#if LWIP_COMPAT_MUTEX == 0

err_t sys_mutex_new(sys_mutex_t *mutex)
{
	*mutex = xSemaphoreCreateMutex();

	if (*mutex == NULL) {
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
	vQueueDelete(*mutex);
}

void sys_mutex_lock(sys_mutex_t *mutex)
{
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

// Task
sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread , void *arg, int stacksize, int prio)
{
	sys_thread_t handle;
	xTaskCreate((TaskFunction_t)thread, (const portCHAR *)name, stacksize,
				arg, (UBaseType_t)(3 + prio), (TaskHandle_t*)(&handle));
	return handle;
}

sys_prot_t sys_arch_protect(void)
{
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
