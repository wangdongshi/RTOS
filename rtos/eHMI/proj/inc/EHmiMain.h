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
#include "SCColor.h"
#include "SCDrawCommand.h"
#include "SCBoard.h"
#include "EHmiEvent.h"

#ifndef __EHMI_MAIN_H__
#define __EHMI_MAIN_H__

enum _SCRENN_ID {
	SCREEN_NONE		= -1,
	SCREEN_TEST1,
	SCREEN_TEST2,
	SCREEN_TEST3,
};

///
/// class : EHmiMain
/// HMI main class
///
class EHmiMain {
public :
    EHmiMain();
    virtual ~EHmiMain();

	void Start(void) {run();}
    bool IsReady(void) {return(is_ready);}
    void SetReady(bool ready) {is_ready = ready;}
    std::mutex& Mutex(void) {return(mtx);}
    std::condition_variable& ConditionVariable(void) {return(cv);}
    void AddQueue(EHmiEvent ev) {deq.push_back(ev);}

private :
	void run(void);
	void main(void);
	void eventHandler(EHmiEvent& ev);

public :
	void StartScreen(void);
	void ChangeScreen(const short id, const EHmiEvent& ev);
	SCBoard* GetBoard(){return(m_screen);}

private :
    std::deque<EHmiEvent>	deq;
    std::mutex				mtx;
    std::condition_variable	cv;
    bool                	is_ready;

	short               	m_screen_id;
	SCBoard*            	m_screen;
};

#endif // __EHMI_MAIN_H__
