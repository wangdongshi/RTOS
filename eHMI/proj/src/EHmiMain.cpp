///
/// file		EHmiMain.cpp
/// brief		HMI main process file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/11/26
///
#include <iostream>
#include <unistd.h>
#include <termio.h>
#include "debug.h"
#include "EHmiMain.h"

using namespace std;

/// function	EHmiMain
/// brief		constructor
EHmiMain::EHmiMain() : is_ready(false)
{
}

/// function	~EHmiMain
/// brief		deconstructor
EHmiMain::~EHmiMain()
{
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
			sleep(1);
            {
                lock_guard<mutex> lock(mtx);
                if(deq.empty()) {
                    // if queue is empty, break while.
                    break;
                }
                else {
                    // get event from queue
					ev = deq.front();
                    deq.pop_front();
                }
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

    // get event type & param
	type = ev.GetEvent();
    switch(type) {
    case HMI_EV_KEYDOWN:
        param = ev.GetULArg();
		cout << endl << "\"" << (char)param << "\" key is pushed down!" << endl;
        break;
    default: // HMI_EV_NONE or not defined
        break;
    }
}
