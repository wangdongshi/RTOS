/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename:  EHmiMain.cpp
 * Project:   Minimum RTOS platform
 * Date:	  2018/11/26
 * Author:	WangYu
 *
 **********************************************************************/
#include "EHmiMain.h"
#include "SCDrawCommand.h"
#include "PICTest1.h"

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

void EHmiMain::run(void)
{
	// TODO : initialize HMI resource
	startScreen(); // create default screen
	main(); // main loop
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
		changeScreen(static_cast<short>(ev.GetULong()), ev);
		m_screen->Draw();
		break;
	default:
		break;
	}
}

void EHmiMain::startScreen(void)
{
	InitializeScreen();

	EHmiEvent ev(HMI_EV_SCREEN_CHG, SCREEN_TEST1);
	xSemaphoreTake(this->Mutex(), 0);
	this->SendQueue(ev);
	xSemaphoreGive(this->Mutex());
}

void EHmiMain::changeScreen(const short id, const EHmiEvent& ev)
{
	// destory old screen
	if(m_screen != NULL) {
		delete m_screen;
		m_screen = NULL;
	}

	// create new screen
	SCRect area(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	switch(id) {
		case SCREEN_TEST1:
			m_screen = new PICTest1(area, id);
			break;
		default:
			break;
	}
}

