///
/// file		EHmiMain.h
/// brief		HMI main process file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/11/26
///
#include <queue>
#include <mutex>
#include <condition_variable>
#include <X11/Xlib.h>
#include "SCDrawContext.h"
#include "EHmiEvent.h"

#ifndef __EHMI_MAIN_H__
#define __EHMI_MAIN_H__

// base window size
enum SCWindowSize {
	WINDOW_WIDTH	= 320,
	WINDOW_HEIGHT	= 240,
};

///
/// class : EHmiMain
/// HMI main class
///
class EHmiMain {
public :
    EHmiMain();
    EHmiMain(Display* display, Window& window);
    virtual ~EHmiMain();
	
public :
	void Start(void) {main();}
    bool IsReady(void) {return(is_ready);}
    void SetReady(bool ready) {is_ready = ready;}
    std::mutex& Mutex(void) {return(mtx);}
    std::condition_variable& ConditionVariable(void) {
		return(cv);
	}
    void AddQueue(EHmiEvent ev) {deq.push_back(ev);}
	
private :
	void main(void);
	void eventHandler(EHmiEvent& ev);

private :
    std::deque<EHmiEvent>	deq;
    std::mutex				mtx;
    std::condition_variable	cv;
    bool                	is_ready;
	SCDrawContext*			dc;
};

#endif // __EHMI_MAIN_H__
