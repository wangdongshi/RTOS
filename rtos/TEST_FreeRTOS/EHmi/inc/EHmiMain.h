/**********************************************************************
 * Copyright (c) 2018 - 2021 by Wang Yu
 * All rights reserved
 *
 * Filename:  EHmiMain.h
 * Project:   Minimum RTOS platform
 * Date:      2018/11/26
 * Author:    Wang Yu
 *
 **********************************************************************/
#ifndef __EHMI_MAIN_H__
#define __EHMI_MAIN_H__

//#include <queue>
//#include <mutex>
//#include <condition_variable>
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "EHmiEvent.h"

///
/// class : EHmiMain
/// HMI main class
///
class EHmiMain {
public :
    EHmiMain();
    virtual ~EHmiMain();
	
public :
	void Start(void) {main();}
    bool IsReady(void) {return(is_ready);}
    void SetReady(bool ready) {is_ready = ready;}
    //std::mutex& Mutex(void) {return(mtx);}
    //std::condition_variable& ConditionVariable(void) {
	//	return(cv);
	//}
    QueueHandle_t Mutex(void) {return(mtx);}
    void AddQueue(EHmiEvent ev);
	
private :
	void main(void);
	void eventHandler(EHmiEvent& ev);

private :
    //std::deque<EHmiEvent>	deq;
    //std::mutex				mtx;
    //std::condition_variable	cv;
    bool                	is_ready;
	QueueHandle_t			mtx;
	QueueHandle_t			deq;
};

#endif // __EHMI_MAIN_H__
