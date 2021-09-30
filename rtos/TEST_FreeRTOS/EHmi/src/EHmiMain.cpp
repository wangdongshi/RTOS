/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename:  EHmiMain.cpp
 * Project:   Minimum RTOS platform
 * Date:      2018/11/26
 * Author:    WangYu
 *
 **********************************************************************/
#include "EHmiMain.h"

#define HMI_EVENT_PROCESS_INTERVAL	(100)	// 100ms
#define HMI_EVENT_QUEUE_DEPTH		(10)

EHmiMain::EHmiMain() :
is_ready(false)
{
	mtx = xSemaphoreCreateMutex();
	deq = xQueueCreate(HMI_EVENT_QUEUE_DEPTH, sizeof(EHmiEvent));
}

EHmiMain::~EHmiMain()
{
}

void EHmiMain::AddQueue(EHmiEvent ev)
{
	xQueueSend(deq, &ev, 0);
}

void EHmiMain::main(void)
{
	EHmiEvent ev(HMI_EV_NONE);
	// command process
	while(true) {
		vTaskDelay(HMI_EVENT_PROCESS_INTERVAL); // HMI react per 100ms
		xSemaphoreTake(this->Mutex(), 0);
		if(uxQueueMessagesWaiting(deq) == 0) { // if queue is empty, break while.
			xSemaphoreGive(this->Mutex());
			continue;
		}
		else {
			xQueueReceive(deq, &ev, 0); // get event for HMI
			xSemaphoreGive(this->Mutex());
			eventHandler(ev); // handle events
		}
	}
}

void EHmiMain::eventHandler(EHmiEvent& ev)
{
    EHmiEventType type = HMI_EV_NONE;
	uint64_t param;
	uint16_t x, y;

	type = ev.GetEvent(); // get event type & parameter
    switch(type) {
    case HMI_EV_KEYDOWN:
        param = ev.GetULArg();
        x = param >> 16 & 0xFFFF;
        y = param & 0xFFFF;
        printf("Finger press (x=%d, y=%d)\r\n", x, y);
        break;
    default: // HMI_EV_NONE or not defined
        break;
    }
}
