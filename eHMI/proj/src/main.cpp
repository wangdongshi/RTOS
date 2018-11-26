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
#include "debug.h"
#include "EHmiMain.h"

using namespace std;

/// function	hmiCtrl
/// brief		eHMI controller function
///
/// param		none
/// return		none
void hmiCtrl()
{
	Trace("HMI ctrl thread is setup!\n");
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
	// create hmi ctrl thread
	// thread hmi_ctrl(hmiCtrl);
	// hmi_ctrl.join();

	// create hmi main thread
	EHmiMain* pHmi = new EHmiMain();
	thread hmi_main(hmiMain, pHmi);
	hmi_main.detach();

	while(1) {
		sleep(1);
		
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
	}

	return 0;
}
