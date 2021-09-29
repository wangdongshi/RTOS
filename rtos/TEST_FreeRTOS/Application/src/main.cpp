/**********************************************************************
 * Copyright (c) 2018 - 2021 by Wang Yu
 * All rights reserved
 *
 * Filename:  main.c
 * Project:   Minimum RTOS platform
 * Date:      2021/9/5
 * Author:    Wang Yu
 *
 **********************************************************************/
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "assert_param.h"
#include "stm32f746g_disco.h"
#include "FreeRTOS.h"
#include "task.h"

static void printBanner(void);
static uint32_t checkDevices(void);

void startTask(void *pvParameters);
void mainTask(void *pvParameters);
void led1Task(void *pvParameters);
#ifdef MODE_STAND_ALONE
void executeCmd(const char* cmd);
#endif

TaskHandle_t startTaskHandler;

int main(void)
{
	// create start task
	//xTaskCreate(startTask,	"START_TASK",	400,	NULL,	2,	&startTaskHandler);
	xTaskCreate(led1Task,	"LED1_TASK",	400,	NULL,	2,	NULL);
	xTaskCreate(mainTask,	"MAIN_TASK",	400,	NULL,	5,	NULL);

	// start FreeRTOS kernel
	vTaskStartScheduler();

	// It should not execute to here
	printf("RTOS task schedule ERROR!!!\n");

	return 0;
}

void startTask(void *pvParameters)
{
	// create task
	taskENTER_CRITICAL();
	xTaskCreate(led1Task,	"LED1_TASK",	400,	NULL,	2,	NULL);
	xTaskCreate(mainTask,	"MAIN_TASK",	400,	NULL,	5,	NULL);
	vTaskDelete(startTaskHandler);
	taskEXIT_CRITICAL();
}

void led1Task(void *pvParameters)
{
	while(1) {
		toggleLED1();
		vTaskDelay(500);
	}
}

void mainTask(void *pvParameters)
{
	// initialization
	printBanner();
	showLogo();
	vTaskDelay(400); // wait FT5336 stable
	checkDevices();

	// main loop
	while(1) {
		vTaskDelay(2000);
	}
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

static uint32_t checkDevices(void)
{
	//printf("Check FPU print with float value 99.99 (Display as %.2f).\r\n", 99.99f);

	//assert_param(checkSDRAM());
	if (!checkSDRAM()) {
		printf("SDRAM initialization failure!\r\n");
		return 0;
	}

	if (!checkTouchPanel()) {
		printf("Touch panel initialization failure!\r\n");
		return 0;
	}

#ifdef MODE_TEST_DRIVER
	uint32_t random = 0x0000FFFF & getRandomData();
	if (!checkDMA(random)) {
		printf("DMA(M2M) transfer failure!\r\n");
		return 0;
	}
#endif

#ifdef MODE_TEST_DRIVER
	checkDMA2D();
#endif

	return 1;
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
