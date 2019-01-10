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
#include "PICTest1.h"
#include "EHmiMain.h"

using namespace std;

/// function	EHmiMain
/// brief		constructor
EHmiMain::EHmiMain() :
is_ready(false),
m_screen_id(SCREEN_NONE),
m_screen(NULL)
{
}

/// function	~EHmiMain
/// brief		deconstructor
EHmiMain::~EHmiMain()
{
}

/// function	run
/// brief		HMI run function(init)
///
/// param		none
/// return		none
void EHmiMain::run(void)
{
	// initialize hmi resource
	
	// create default screen
	startScreen();

	// main loop
	main();
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
			usleep(10);
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
/// param		ev		event popped from queue
/// return		none
void EHmiMain::eventHandler(EHmiEvent& ev)
{
    EHmiEventType	type = HMI_EV_NONE;
	//unsigned long param;
	unsigned int	x, y;
	
    // get event type & param
	type = ev.GetEvent();
	ev.GetParam(&x, &y);
	SCPoint point(x, y);
	
	// dispatch event
    switch(type) {
    case HMI_EV_WINDOW_INIT:
		Trace("Get window initialization event.\n");
        break;
    case HMI_EV_EXPOSE:
		Trace("Get window expose event.\n");
		m_screen->Draw();
        break;
    case HMI_EV_MOUSE_DOWN:
		Trace("Get mouse down event. X=%d, Y=%d\n", point.x, point.y);
		m_screen->TDown(point);
        break;
    case HMI_EV_MOUSE_UP:
		Trace("Get mouse up   event. X=%d, Y=%d\n", point.x, point.y);
		m_screen->TUp(point);
        break;
	case HMI_EV_MOUSE_MOVE:
		Trace("Get mouse move event. X=%d, Y=%d\n", point.x, point.y);
		m_screen->TMove(point);
        break;
    default: // HMI_EV_NONE or not defined
        break;
    }
}

/// function	startScreen
/// brief		initialize default screen
///
/// param		none
/// return		none
void EHmiMain::startScreen(void)
{
	EHmiEvent ev(HMI_EV_CHG_SCREEN);

	changeScreen(SCREEN_TEST1, ev);
}

/// function	changeScreen
/// brief		send change screen message
///
/// param		id		screen ID
/// param		ev		event message
/// return		none
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
		default:
		case SCREEN_TEST1:
			 m_screen = new PICTest1(area, id);
			 break;
	}
}

