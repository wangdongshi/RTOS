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
#include "PICTest2.h"

#define HMI_EVENT_PROCESS_INTERVAL	(100)	// 100ms
#define HMI_EVENT_QUEUE_DEPTH		(10)

EHmiMain::EHmiMain() :
is_ready(false),
m_screen_id(SCREEN_NONE),
m_screen(NULL)
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
	type = ev.GetEvent();

	unsigned int ui1 = ev.GetUInt(0);
	unsigned int ui2 = ev.GetUInt(1);
	SCPoint point(ui1, ui2);

	switch(type) {
	case HMI_EV_NONE:
	case HMI_EV_DATA_UPDATE:
		break;
	case HMI_EV_CYCLIC_REFRESH:
		m_screen->Update();
		break;
	case HMI_EV_TOUCH_DOWN:
		TRACE("Touch Down\t(x=%d, y=%d).\r\n", point.x, point.y);
		m_screen->TDown(point);
		break;
	case HMI_EV_TOUCH_UP:
		TRACE("Touch Up\t(x=%d, y=%d).\r\n", point.x, point.y);
		m_screen->TUp(point);
		break;
	case HMI_EV_TOUCH_MOVE:
		TRACE("Touch Move\t(x=%d, y=%d).\r\n", point.x, point.y);
		m_screen->TMove(point);
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

	SetReady(true);
}

void EHmiMain::changeScreen(const short id, const EHmiEvent& ev)
{
	// destroy old screen
	if(m_screen != NULL) {
		delete m_screen;
		m_screen = NULL;
	}

	// create new screen
	SCRect area(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	switch(id) {
		default:
		case SCREEN_TEST1:
			m_screen = new PICTest1(area, id);
			break;
		case SCREEN_TEST2:
			m_screen = new PICTest2(area, id);
			break;
	}
}

