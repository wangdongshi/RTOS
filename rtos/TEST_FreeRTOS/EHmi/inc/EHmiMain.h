/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename:  EHmiMain.h
 * Project:   Minimum RTOS platform
 * Date:      2018/11/26
 * Author:    WangYu
 *
 **********************************************************************/
#ifndef __EHMI_MAIN_H__
#define __EHMI_MAIN_H__

#include "platform.h"
#include "EHmiEvent.h"

class EHmiMain {
public :
    EHmiMain();
    virtual ~EHmiMain();
	
public :
	void Start(void) {main();}
    bool IsReady(void) {return(is_ready);}
    void SetReady(bool ready) {is_ready = ready;}
    QueueHandle_t Mutex(void) {return(mtx);}
    void SendQueue(EHmiEvent ev);
    void SendQueueFromISR(EHmiEvent ev);
	
private :
	void main(void);
	void eventHandler(EHmiEvent& ev);

private :
    bool			is_ready;
	QueueHandle_t	mtx;
	QueueHandle_t	deq;
};

#endif // __EHMI_MAIN_H__
