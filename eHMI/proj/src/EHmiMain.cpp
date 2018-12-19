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
#include <X11/Xlib.h>
#include "debug.h"
#include "SCColor.h"
#include "SCDrawContext.h"
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
dc(new SCDrawContext(display, window)),
color(new SCColor(display))
{
	color->AllocColor();
}

/// function	~EHmiMain
/// brief		deconstructor
EHmiMain::~EHmiMain()
{
	delete dc;
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
	//unsigned long param;
	unsigned int  x, y;
	
    // get event type & param
	type = ev.GetEvent();
    switch(type) {
    case HMI_EV_WINDOW_INIT:
		Trace("Get window init event.\n");
        break;
    case HMI_EV_EXPOSE:
		Trace("Get window expose event.\n");
	{
		char str[] = "Welcome to this embedded HMI sample!";
		char* p_str = str;
		SCPos pos(10, 30);
		unsigned int x = 35, y = 40, internal = 7;
		// draw a rectangle
		dc->drawRect(pos, 300, 80, GetColor("Blue"));
		// draw a string
		while (*p_str) {
			dc->drawASCII(x, y, static_cast<const char>(*p_str), GetColor("Black"), GetColor("White"), SC_FONT_MIDDLE);
			p_str++;
			x += internal;
		}
	}
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
