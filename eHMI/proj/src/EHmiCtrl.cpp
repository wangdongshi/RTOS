///
/// file		EHmiCtrl.cpp
/// brief		HMI control process file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/12/07
///
#include <mutex>
#include <unistd.h>
#include <termio.h>
#include "debug.h"
#include "EHmiCtrl.h"

using namespace std;

#define MOUSE_MOVE_ACT_COUNT	4

/// function	EHmiCtrl
/// brief		constructor
EHmiCtrl::EHmiCtrl() : 
hmi(NULL),
move_cnt(0)
{
}

/// function	EHmiCtrl
/// param		display
/// param		pHmi
/// brief		constructor
EHmiCtrl::EHmiCtrl(EHmiMain* pHmi) : 
hmi(pHmi),
move_cnt(0)
{
}

/// function	~EHmiMain
/// brief		deconstructor
EHmiCtrl::~EHmiCtrl()
{
}

/// function	main
/// brief		HMI touch event monitor function
///
/// param		none
/// return		none
void EHmiCtrl::main(void)
{
	Display* disp = SCDrawContext::GetDisplay();

	// init window
	EHmiEvent hmi_ev(HMI_EV_WINDOW_INIT);
	send2HMI(hmi_ev);

	// catch and process event created from screen
    while(true) {
		usleep(10);
		// event get from mouse
		XEvent win_ev;
		XNextEvent(disp, (XEvent *)&win_ev);
		// send event
		switch(win_ev.type) 
		{
			case Expose: // window initialization event
			{
				EHmiEvent hmi_ev(HMI_EV_EXPOSE);
				send2HMI(hmi_ev);
			}
				break;
			case ButtonPress: // mouse down event
			{
				XButtonEvent xbutton = win_ev.xbutton;
				EHmiEvent hmi_ev(HMI_EV_MOUSE_DOWN, xbutton.x, xbutton.y);
				send2HMI(hmi_ev);
			}
				break;
			case ButtonRelease: // mouse up event
			{
				XButtonEvent xbutton = win_ev.xbutton;
				EHmiEvent hmi_ev(HMI_EV_MOUSE_UP, xbutton.x, xbutton.y);
				send2HMI(hmi_ev);
			}
				break;
			case MotionNotify: // mouse move
			{
				// the mouse movement sampling rate can adjust by the MOUSE_MOVE_ACT_COUNT
				if (move_cnt >= MOUSE_MOVE_ACT_COUNT) {
					XButtonEvent xbutton = win_ev.xbutton;
					EHmiEvent hmi_ev(HMI_EV_MOUSE_MOVE, xbutton.x, xbutton.y);
					send2HMI(hmi_ev);
					move_cnt = 0;
				} else {
					++move_cnt;
				}
			}
				break;
			default:
				break;
		}
    }
}


/// function	send2HMI
/// brief		push event to hmi main thread
///
/// param		ev
/// return		none
void EHmiCtrl::send2HMI(EHmiEvent& ev)
{
	lock_guard<mutex> lock(hmi->Mutex());
	hmi->SetReady(true);
	hmi->AddQueue(ev);
}

