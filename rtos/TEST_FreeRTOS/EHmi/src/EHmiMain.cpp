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

void EHmiMain::SendQueue(EHmiEvent ev)
{
	xQueueSend(deq, &ev, 0);
}

void EHmiMain::SendQueueFromISR(EHmiEvent ev)
{
	xQueueSendFromISR(deq, &ev, 0);
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
	uint16_t x, y;

	type = ev.GetEvent();
    switch(type) {
    case HMI_EV_NONE:
    case HMI_EV_DATA_UPDATE:
    case HMI_EV_CYCLIC_REFRESH:
    	break;
    case HMI_EV_TOUCH_DOWN:
        x = ev.GetUInt(0);
        y = ev.GetUInt(1);
        TRACE("Touch Down\t(x=%d, y=%d).\r\n", x, y);
        break;
    case HMI_EV_TOUCH_UP:
        x = ev.GetUInt(0);
        y = ev.GetUInt(1);
        TRACE("Touch Up\t(x=%d, y=%d).\r\n", x, y);
        break;
    case HMI_EV_TOUCH_MOVE:
        x = ev.GetUInt(0);
        y = ev.GetUInt(1);
        TRACE("Touch Move\t(x=%d, y=%d).\r\n", x, y);
        break;
    case HMI_EV_SCREEN_CHG:
    default:
        break;
    }
}
