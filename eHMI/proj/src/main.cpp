///
/// file		main.cpp
/// brief		shell function for testing embedded HMI
///
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/11/26
///
#include <thread>
#include <unistd.h>
#include "debug.h"

using namespace std;

/// function	hmiCtrl
/// brief		eHMI controller function
/// param		none
/// return		none
void hmiCtrl()
{
	trace("hmi ctrl thread is setup!\n");
}

/// function	hmiMain
/// brief		eHMI main function
/// param		none
/// return		none
void hmiMain()
{
	trace("hmi main thread is setup!\n");
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
	thread hmi_ctrl(hmiCtrl);
	hmi_ctrl.join();

	// create hmi main thread
	thread hmi_main(hmiMain);
	hmi_main.join();

	while(1) {
		sleep(1);
	}

	return 0;
}
