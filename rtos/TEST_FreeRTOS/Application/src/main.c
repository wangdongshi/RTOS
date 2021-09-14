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
#include <stdio.h>
#include "assert_param.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f746g_disco_driver.h"

#define SDRAM_SIZE			(0x800000UL)

void startTask(void *pvParameters);
void mainTask(void *pvParameters);
void led1Task(void *pvParameters);
bool checkSDRAM(void);
#ifdef MODE_STAND_ALONE
void executeCmd(const char* cmd);
#endif

TaskHandle_t startTaskHandler;

// Main function for LED test
int main(void)
{
	// Initialize board
	initBoard();

	// create start task
	//xTaskCreate(startTask,	"START_TASK",	400,	NULL,	2,	&startTaskHandler);
	xTaskCreate(led1Task,	"LED1_TASK",	400,	NULL,	2,	NULL);
	xTaskCreate(mainTask,	"MAIN_TASK",	400,	NULL,	5,	NULL);

	// start FreeRTOS kernel
	vTaskStartScheduler();

	// It should not execute to here
	printf("RTOS task schedule ERROR !!!\n");

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
	// print shell banner
	printf("Welcome to STM32F746G-DISCO !\r\n");
	//printf("Test FPU function with float value(%.4f). \r\n", 99.99f);

	// test SDRAM
	//assert_param(checkSDRAM());
	if (!checkSDRAM()) {
		printf("SDRAM initialization Failure !\r\n");
		return;
	}

	// main loop
	while(1) {
		vTaskDelay(50);
	}
}

bool checkSDRAM(void)
{
	uint8_t* p;

	// check after write
	for (p = (uint8_t *)SDRAM_BANK1; (uint32_t)p < SDRAM_BANK1 + SDRAM_SIZE; p += 0x100000) {
		*p = 0xA5;
		if ((*p) != 0xA5) return FALSE;
	}
	p = (uint8_t *)(SDRAM_BANK1 + SDRAM_SIZE - 1);
	*p = 0x5A;
	if ((*p) != 0x5A) return FALSE;

	// check delay
	for (volatile uint32_t i = 0; i < 1000000; i++);
	for (p = (uint8_t *)SDRAM_BANK1; (uint32_t)p < SDRAM_BANK1 + SDRAM_SIZE; p += 0x100000) {
		if ((*p) != 0xA5) return FALSE;
	}
	p = (uint8_t *)(SDRAM_BANK1 + SDRAM_SIZE - 1);
	if ((*p) != 0x5A) return FALSE;

	return TRUE;
}

#ifdef MODE_STAND_ALONE

#define BUF_SIZE		(1000)
char character = 0;
static char buffer[BUF_SIZE];

void main(void)
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
