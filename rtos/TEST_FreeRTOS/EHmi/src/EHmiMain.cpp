/**********************************************************************
 * Copyright (c) 2018 - 2021 by Wang Yu
 * All rights reserved
 *
 * Filename:  EHmiMain.cpp
 * Project:   Minimum RTOS platform
 * Date:      2018/11/26
 * Author:    Wang Yu
 *
 **********************************************************************/
#include "debug.h"
#include "EHmiMain.h"

//using namespace std;

/// function	EHmiMain
/// brief		constructor
EHmiMain::EHmiMain() :
is_ready(false)
{
	mtx = xSemaphoreCreateMutex();
	deq = xQueueCreate(20, sizeof(EHmiEvent));
}

/// function	~EHmiMain
/// brief		deconstructor
EHmiMain::~EHmiMain()
{
}

void EHmiMain::AddQueue(EHmiEvent ev)
{
	//deq.push_back(ev);
	xQueueSend(deq, &ev, 0);
}

/// function	main
/// brief		HMI main process function
///
/// param		none
/// return		none
void EHmiMain::main(void)
{
    EHmiEvent ev(HMI_EV_NONE);
	
    while(true) {
        // {
            // unique_lock<mutex> lock(mtx);
            // cv.wait(lock, [this]{return is_ready;});
            // is_ready = false;
        // }

        // command process
        while(true) {
			//sleep(1);
        	vTaskDelay(100);
            {
                //lock_guard<mutex> lock(mtx);
                //if(deq.empty()) {
        		xSemaphoreTake(mtx, 0);
                if(uxQueueMessagesWaiting(deq) == 0) {
                    // if queue is empty, break while.
                    break;
                }
                else {
                    // get event from queue
					//ev = deq.front();
                    //deq.pop_front();
                	xQueueReceive(deq, &ev, 0);
                }
                xSemaphoreGive(mtx);
            }
            // event process
            eventHandler(ev);
        }
    }
}

/// function	eventHandler
/// brief		HMI main event process function
///
/// param		ev	: event popped from queue
/// return		none
void EHmiMain::eventHandler(EHmiEvent& ev)
{
    EHmiEventType type = HMI_EV_NONE;
	unsigned long param;
	uint16_t x, y;

    // get event type & param
	type = ev.GetEvent();
    switch(type) {
    case HMI_EV_KEYDOWN:
        param = ev.GetULArg();
        x = param >> 16 & 0xFFFF;
        y = param & 0xFFFF;
        TRACE("key is pushed down! (x=%d, y=%d)\r\n", x, y);
        break;
    default: // HMI_EV_NONE or not defined
        break;
    }
}
