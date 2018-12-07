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

/// function	EHmiMain
/// brief		constructor
EHmiMain::EHmiMain(Display* display, Window& window) : 
is_ready(false),
disp(display),
win(window),
gc(XCreateGC(display, window, 0, NULL))
{
}

/// function	~EHmiMain
/// brief		deconstructor
EHmiMain::~EHmiMain()
{
	XFreeGC(disp, gc);
}

/// function	main
/// brief		HMI main process function
///
/// param		none
/// return		none
void EHmiMain::main(void)
{
	// get draw context
	gc = XCreateGC(disp, win, 0, NULL);

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
                } else {
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
	unsigned int  x, y;

    // get event type & param
	type = ev.GetEvent();
    switch(type) {
    case HMI_EV_INIT:
		Trace("Get window init event.\n");
        break;
    case HMI_EV_MOUSE_DOWN:
		ev.GetParam(&x, &y);
		Trace("Get mouse down event. X=%d, Y=%d\n", x, y);
        break;
    case HMI_EV_MOUSE_UP:
		ev.GetParam(&x, &y);
		Trace("Get mouse up   event. X=%d, Y=%d\n", x, y);
        break;
    default: // HMI_EV_NONE or not defined
        break;
    }
}
