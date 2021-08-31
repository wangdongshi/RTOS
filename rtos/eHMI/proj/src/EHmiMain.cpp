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
#include "PICTest2.h"
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
	// initialize HMI resource
	
	// create default screen
	StartScreen();

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
	// get event type & parameter
    EHmiEventType type = HMI_EV_NONE;
	type = ev.GetEvent();
	
	short screen_id = ev.GetParam();
	unsigned int ui1, ui2;
	ev.GetParam(&ui1, &ui2);
	SCPoint point(ui1, ui2);
	
	EHmiEvent event;
	
	// dispatch event
    switch(type) {
    case HMI_EV_EXPOSE:
		//Trace("Get window expose event.\n");
		m_screen->Draw();
        break;
    case HMI_EV_WINDOW_INIT:
		//Trace("Get window initialization event.\n");
        break;
    case HMI_EV_TIMER_1S:
#if 0
	{
		time_t tt = GetTime();
		tm* t= gmtime(&tt);

		Trace("Current Time is %d-%02d-%02d %02d:%02d:%02d.\n",
				t->tm_year + 1900,
				t->tm_mon + 1,
				t->tm_mday,
				t->tm_hour,
				t->tm_min,
				t->tm_sec);
		
	}
#endif
		m_screen->Update();
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
    case HMI_EV_CHG_SCREEN:
		//Trace("Get change screen event. (screen_id = %d)\n", screen_id);
        ChangeScreen(screen_id, event);
		m_screen->Draw();
        break;
    default: // HMI_EV_NONE or not defined
        break;
    }
}

/// function	StartScreen
/// brief		initialize default screen
///
/// param		none
/// return		none
void EHmiMain::StartScreen(void)
{
	EHmiEvent ev(HMI_EV_CHG_SCREEN);

	ChangeScreen(SCREEN_TEST1, ev);
}

/// function	ChangeScreen
/// brief		send change screen message
///
/// param		id		screen ID
/// param		ev		event message
/// return		none
void EHmiMain::ChangeScreen(const short id, const EHmiEvent& ev)
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

