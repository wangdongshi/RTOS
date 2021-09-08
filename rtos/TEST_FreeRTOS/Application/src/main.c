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
#include <string.h>
#include "stm32f746g_disco_driver.h"
#include "FreeRTOS.h"
#include "task.h"

#define BUF_SIZE		(1000)

typedef unsigned long	uint32_t;

char character = 0;
static char buffer[BUF_SIZE];
TaskHandle_t startTaskHandler;

void startTask(void *pvParameters);
void ledTask(void *pvParameters);
void shellTask(void *pvParameters);
void executeCmd(const char* cmd);

// Main function for LED test
int main(void)
{
	// Initialize board
	initBoard();

	// create start task
	xTaskCreate(startTask,	"START_TASK",	400,	NULL,	2,	&startTaskHandler);

	// start FreeRTOS kernel
	vTaskStartScheduler();

	// It should not execute to here
	usart1SendBuffer("RTOS task schedule ERROR !!!\r#");

	return 0;
}

void startTask(void *pvParameters)
{
	// create task
	taskENTER_CRITICAL();
	xTaskCreate(ledTask,	"LED_TASK",		400,	NULL,	2,	NULL);
	xTaskCreate(shellTask,	"SHELL_TASK",	400,	NULL,	5,	NULL);
	vTaskDelete(startTaskHandler);
	taskEXIT_CRITICAL();
}

void ledTask(void *pvParameters)
{
	while(1) {
		toggleLED1();
		vTaskDelay(500);
	}
}

void shellTask(void *pvParameters)
{
	uint32_t index = 0;

	// print shell banner
	usart1SendBuffer("Welcome to STM32F746G-DISCO !\r#");

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
				usart1SendChar('#'); // print next prompt
				index = 0;
			}
			character = 0;
		}
		vTaskDelay(50);
	}
}

// Command processing function
void executeCmd(const char* cmd)
{
	uint32_t len = strlen(cmd) - 1;
	if (strncmp(cmd, "help", len) == 0) {
		usart1SendBuffer("This is help command.\r\r");
	} else {
		usart1SendBuffer("This is a wrong command.\r\r");
	}
}
