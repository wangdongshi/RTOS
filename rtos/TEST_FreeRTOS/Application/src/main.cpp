/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : main.c
 * Project  : Minimum RTOS platform
 * Date     : 2021/9/5
 * Author   : WangYu
 *
 **********************************************************************/
extern"C"
{
#include "libdev.h"
}

#include "platform.h"
#include "EHmiMain.h"

int main(void);
void startTask(void *pvParameters);
void mainTask(void *pvParameters);
void ehmiTask(void *pvParameters);
void led1Task(void *pvParameters);
#ifdef MODE_STAND_ALONE
void executeCmd(const char* cmd);
#endif

static bool_t checkDevices(void);
static void printBanner(void);

EHmiMain* pHmi = new EHmiMain();

int main(void)
{
	// create start task
	xTaskCreate(startTask,	"START_TASK",	400,	NULL,	2,	NULL);

	// start FreeRTOS kernel
	vTaskStartScheduler();

	// It should not execute to here
	TRACE("RTOS task schedule ERROR!!!\r\n");

	return 0;
}

void startTask(void *pvParameters)
{
	// create system resource
	taskENTER_CRITICAL();
	xTaskCreate(led1Task,	"LED1_TASK",	400,	NULL,	2,	NULL); // for monitor board
	xTaskCreate(ehmiTask,	"HMI_TASK",		400,	NULL,	2,	NULL);
	xTaskCreate(mainTask,	"MAIN_TASK",	400,	NULL,	5,	NULL);
	TaskHandle_t handler = xTaskGetHandle("START_TASK");
	vTaskDelete(handler);
	taskEXIT_CRITICAL();
}

void led1Task(void *pvParameters)
{
	xEventGroupSetBits(pHmi->EventFlag(), TASK_LED1_READY_EVENT);

	while(1) {
		//toggleLED1();
		toggle_LED1_from_lib(); // for test static library
		vTaskDelay(500);
	}
}

void ehmiTask(void * pvParameters)
{
	(static_cast<EHmiMain*>(pHmi))->Start();
}

void mainTask(void *pvParameters)
{
	// initialization
	printBanner();
	showLogo();
	checkDevices();

	// send event flag to EHMI task
	TaskHandle_t handler = xTaskGetHandle("MAIN_TASK");
	xEventGroupSetBits(pHmi->EventFlag(), TASK_MAIN_READY_EVENT);

	// test file system
	/*
	FATFS fs;
	FIL fp;
	if (!f_mount(&fs, "1:", 1)) {
		TRACE("File system mount is success !\r\n");
	}
	else {
		TRACE("File system mount is failed !\r\n");
	}
	*/

	// suspend
	vTaskSuspend(handler);
	/*
	while(1) {
		vTaskDelay(2000);
	}
	*/
}

void* operator new(size_t size)
{
	return pvPortMalloc(size);
}

void* operator new[](size_t size)
{
	return pvPortMalloc(size);
}

void operator delete(void * ptr)
{
	vPortFree (ptr);
}

void operator delete[](void * ptr)
{
	vPortFree (ptr);
}

static bool_t checkDevices(void)
{
	//TRACE("Check FPU print with float value 99.99 (Display as %.2f).\r\n", 99.99f);

	//assert_param(checkSDRAM());
	if (!checkSDRAM()) {
		TRACE("Failed to initialize SDRAM !\r\n");
		return False;
	}

	if (!checkTouchPanel()) {
		TRACE("Failed to initialize Touch panel!\r\n");
		return False;
	}

#ifdef MODE_TEST_DRIVER
	uint16_t random = (uint16_t)(0x0000FFFF & getRandomData());
	//if (!checkDMA(random)) {
	//	TRACE("Failed to initialize DMA(M2M) !\r\n");
	//	return False;
	//}
#endif

#ifdef MODE_TEST_DRIVER
	checkDMA2D();
#endif

#ifdef MODE_TEST_DRIVER
	// Pay attention to this test. It will break the file system !!!
	if (isSDCardInsert()) {
		bool_t res = True;
		if (res) res = setSDCardData(random);
		if (res) res = getSDCardData(random);
		vTaskDelay(100);
		if (res) res = checkSDCardData();
		if (!res) {
			printf("Failed to initialize SD Card !\r\n");
			return False;
		}
	}
#endif

	return True;
}

static void printBanner(void)
{
	// create from "http://patorjk.com/software/taag/"
	printf("^_^ Welcome to STM32F746G-DISCO ^_^\r\n");
	printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\r\n");
	printf("@                                 @\r\n");
	printf("@    @@@@     @@@@                @\r\n");
	printf("@     @@   @@  @@ @@@@   @@@@     @\r\n");
	printf("@      @@ @@@ @@   @@@   @@@      @\r\n");
	printf("@       @@@ @@@     @@@ @@@       @\r\n");
	printf("@        @   @        @@@@        @\r\n");
	printf("@                  @@@@@@         @\r\n");
	printf("@                                 @\r\n");
	printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\r\n");
	printf("\r\n");
}

#ifdef MODE_STAND_ALONE

#define BUF_SIZE		(1000)
char character = 0;
static char buffer[BUF_SIZE];

void main(void)
{
	uint32_t index = 0;

	// print shell banner
	usart1SendBuffer("Welcome to STM32F746G-DISCO!\r#");

	// clear receive buffer
	memset(buffer, 0x00, BUF_SIZE);

	// command loop
	while(1) {
		if (character != 0) { // character has been received
			buffer[index++] = character;
			if (character == '\r') {
				char cmd[256] = "";
				buffer[--index] = '\n';
				strcpy(cmd, buffer); // copy command text
				memset(buffer, 0x00, BUF_SIZE);
				executeCmd(cmd);
				index = 0;
			}
			character = 0;
		}
	}
}

// Command processing function
void executeCmd(const char* cmd)
{
	uint32_t len = strlen(cmd) - 1;
	if (strncmp(cmd, "help", len) == 0) {
		usart1SendBuffer("This is help command.\r\r#");
	} else {
		usart1SendBuffer("This is a wrong command.\r\r#");
	}
}
#endif
