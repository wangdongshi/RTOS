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
extern int libEntry(void);
}

#include <string.h>
#include "platform.h"
#include "EHmiMain.h"
#include "ethernetif.h"
#include "http_task.h"
#include "mqtt_task.h"

//#define TEST_DMA
//#define TEST_DMA2D
//#define TEST_SD_CARD
//#define TEST_FILE_SYSTEM
//#define TEST_DYNAMIC_LOAD

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
#ifdef TEST_FILE_SYSTEM
static void testFileSystem(void);
#endif
#ifdef TEST_DYNAMIC_LOAD
static void testDynamicLoad(void);
#endif

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
	xTaskCreate(ehmiTask,	"EHMI_TASK",	400,	NULL,	2,	NULL);
	xTaskCreate(httpTask,	"HTTP_TASK",	400,	NULL,	3,	NULL);
	xTaskCreate(mqttTask,	"MQTT_TASK",	400,	NULL,	3,	NULL);
	xTaskCreate(mainTask,	"MAIN_TASK",	8000,	NULL,	5,	NULL);
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
	LWIP_Init();
	printBanner();
	showLogo();
	checkDevices();

	// send event flag to EHMI task
	xEventGroupSetBits(pHmi->EventFlag(), TASK_MAIN_READY_EVENT);

#ifdef TEST_FILE_SYSTEM
	// test file system
	testFileSystem();
#endif

#ifdef TEST_DYNAMIC_LOAD
	testDynamicLoad();
#endif

	// suspend
	TaskHandle_t handler = xTaskGetHandle("MAIN_TASK");
	vTaskSuspend(handler);
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

#ifdef TEST_DMA
	if (!checkDMA((uint16_t)(0x0000FFFF & getRandomData())) {
		TRACE("Failed to initialize DMA(M2M) !\r\n");
		return False;
	}
#endif

#ifdef TEST_DMA2D
	checkDMA2D();
#endif

#ifdef TEST_SD_CARD
	// Pay attention to this test. It will break the file system !!!
	if (isSDCardInsert()) {
		bool_t res = True;
		uint32_t addr = (0x000FFFFF & getRandomData()) | 0x8; // avoid the header blocks
		if (res) res = initSDCard();
		if (res) res = setSDCardData(addr);
		if (res) res = getSDCardData(addr);
		if (res) res = checkSDCardData();
		if (!res) {
			printf("Failed to initialize SD Card !\r\n");
			return False;
		}
	}
#endif

	return True;
}

#ifdef TEST_FILE_SYSTEM
static void testFileSystem(void)
{
	FATFS	fs;
	FIL		fp;
	UINT	num;
	char	text[] = "Hello, FatFs!";
	char	buff[32];

	if (f_mount(&fs, "0:", 1)) {
		printf("Failed to mount file system mount !\r\n");
		return;
	}

	if (f_open(&fp, "0:test.txt", FA_CREATE_ALWAYS | FA_WRITE)) {
		printf("Failed to open file !\r\n");
		return;
	}

	if (f_write(&fp, (char*)text, strlen(text), &num)) {
		printf("Failed to write file !\r\n");
		return;
	}

	if (f_close(&fp)) {
		printf("Failed to close file !\r\n");
		return;
	}

	if (f_open(&fp, "0:test.txt", FA_READ)) {
		printf("Failed to open file !\r\n");
		return;
	}

	if (f_read(&fp, (char*)&buff, strlen(text), &num)) {
		printf("Failed to read file !\r\n");
		return;
	}

	if (f_close(&fp)) {
		printf("Failed to close file !\r\n");
		return;
	}

	buff[strlen(text)] = '\0';
	printf("%s\r\n", buff);
}
#endif

#ifdef TEST_DYNAMIC_LOAD
static void testDynamicLoad(void)
{
	libEntry();
	TRACE("Dynamic loading function is OK");
}
#endif

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
