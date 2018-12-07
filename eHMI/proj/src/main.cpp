///
/// file		main.cpp
/// brief		shell function for testing embedded HMI
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/11/26
///
#include <thread>
#include <unistd.h>
#include <termio.h>
#include <X11/Xlib.h>
#include "debug.h"
#include "EHmiCtrl.h"
#include "EHmiMain.h"

using namespace std;

/// function	hmiCtrl
/// brief		eHMI controller function
///
/// param		pCtrl
/// return		none
void hmiCtrl(EHmiCtrl* pCtrl)
{
	Trace("HMI touch event monitor thread is setup!\n");
	pCtrl->Start();
}

/// function	hmiMain
/// brief		eHMI main function
///
/// param		pHmi
/// return		none
void hmiMain(EHmiMain* pHmi)
{
	Trace("HMI main thread is setup!\n");
	pHmi->Start();
}

/// function	main
/// brief		shell function
/// 
/// Cereate two threads. The one is the controller, and the other one
/// is eHMI itself. The controller create event and send to eHMI thread.
/// And the eHMI draw the picture by these events.
///
/// param		none
/// return		error code
int main( void )
{
	// create panel with touch(mouse) event
	// link to X server
	Display* disp = XOpenDisplay(NULL);
	// create 320x240 window
	unsigned long background = WhitePixel(disp, 0);
	unsigned long foreground = BlackPixel(disp, 0);
	Window win = XCreateSimpleWindow(disp, DefaultRootWindow(disp),
			0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, background);
	// select draw event
	XSelectInput(disp, win, ExposureMask | ButtonPressMask | ButtonReleaseMask);
	// map window
	XMapWindow(disp, win);

	// create hmi main thread
	EHmiMain* pHmi = new EHmiMain(disp, win);
	thread hmi_main(hmiMain, pHmi);
	hmi_main.detach();

	// create hmi ctrl thread
	EHmiCtrl* pCtrl = new EHmiCtrl(disp, pHmi);
	thread hmi_ctrl(hmiCtrl, pCtrl);
	hmi_ctrl.detach();

	while(1) {
		sleep(1);
		/*	
		int in;
		struct termios new_settings;
		struct termios stored_settings;
		
		tcgetattr(0, &stored_settings);
		new_settings = stored_settings;
		new_settings.c_lflag &= (~ICANON);
		new_settings.c_cc[VTIME] = 0;
		tcgetattr(0, &stored_settings);
		new_settings.c_cc[VMIN] = 1;
		tcsetattr(0, TCSANOW, &new_settings);
		in = getchar();
		tcsetattr(0, TCSANOW, &stored_settings);
		
		EHmiEvent ev(HMI_EV_KEYDOWN, (unsigned long)in);
		{
			lock_guard<mutex> lock(pHmi->Mutex());
			pHmi->SetReady(true);
			pHmi->AddQueue(ev);
		}
		*/
	}

	XCloseDisplay(disp);
	
	return 0;
}
