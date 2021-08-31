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
#include "EHmiCtrl.h"

#define MOUSE_MOVE_ACT_COUNT	4

using namespace std;

EHmiMain* EHmiCtrl::hmi = NULL;

/// function	EHmiCtrl
/// brief		constructor
EHmiCtrl::EHmiCtrl() : 
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
	SendMassage(hmi_ev);

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
				SendMassage(hmi_ev);
			}
				break;
			case ButtonPress: // mouse down event
			{
				XButtonEvent xbutton = win_ev.xbutton;
				EHmiEvent hmi_ev(HMI_EV_MOUSE_DOWN, xbutton.x, xbutton.y);
				SendMassage(hmi_ev);
			}
				break;
			case ButtonRelease: // mouse up event
			{
				XButtonEvent xbutton = win_ev.xbutton;
				EHmiEvent hmi_ev(HMI_EV_MOUSE_UP, xbutton.x, xbutton.y);
				SendMassage(hmi_ev);
			}
				break;
			case MotionNotify: // mouse move
			{
				// the mouse movement sampling rate can adjust by the MOUSE_MOVE_ACT_COUNT
				if (move_cnt >= MOUSE_MOVE_ACT_COUNT) {
					XButtonEvent xbutton = win_ev.xbutton;
					EHmiEvent hmi_ev(HMI_EV_MOUSE_MOVE, xbutton.x, xbutton.y);
					SendMassage(hmi_ev);
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

/// function	Send2HMI
/// brief		push event to hmi main thread
///
/// param		ev
/// return		none
void EHmiCtrl::Send2HMI(EHmiEvent& ev)
{
	lock_guard<mutex> lock(EHMI->Mutex());
	EHMI->SetReady(true);
	EHMI->AddQueue(ev);
}
