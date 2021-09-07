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

#define BUF_SIZE		(1000)

typedef unsigned int	uint32_t;

char character = 0;
static char buffer[BUF_SIZE];

void executeCmd(const char* cmd);

// main function for LED test
int main(void)
{
	// initialize board
	initBoard();

	// print banner by USART1
	usart1SendBuffer("Welcome to STM32F746G-DISCO !\r#");

	// command processing
	uint32_t index = 0;
	memset(buffer, 0x00, BUF_SIZE);
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
	}

	return 0;
}

void executeCmd(const char* cmd)
{
	uint32_t len = strlen(cmd) - 1;
	if (strncmp(cmd, "help", len) == 0) {
		usart1SendBuffer("This is help command.\r\r");
	} else {
		usart1SendBuffer("This is a wrong command.\r\r");
	}
}
