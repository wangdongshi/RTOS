/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : EHmiMain.h
 * Project  : Minimum RTOS platform
 * Date     : 2018/11/26
 * Author   : WangYu
 *
 **********************************************************************/
#ifndef __EHMI_MAIN_H__
#define __EHMI_MAIN_H__

#include "platform.h"
#include "EHmiEvent.h"
#include "SCBoard.h"

#define TASK_MAIN_READY_EVENT		(0x1 << 0)
#define TASK_LED1_READY_EVENT		(0x1 << 1)
#define TASK_ALL_READY_EVENT		(TASK_MAIN_READY_EVENT | TASK_LED1_READY_EVENT)

enum _SCRENN_ID {
	SCREEN_NONE		= -1,
	SCREEN_TEST1,
	SCREEN_TEST2,
	SCREEN_TEST3,
};

class EHmiMain {
public :
	EHmiMain();
	virtual ~EHmiMain();
	
public :
	void Start(void) {run();}
	bool IsReady(void) {return(is_ready);}
	void SetReady(bool ready) {is_ready = ready;}
	QueueHandle_t Mutex(void) {return(mtx);}
	EventGroupHandle_t EventFlag(void) {return(ev_flag);}
	void SendQueue(EHmiEvent ev);
	void SendQueueFromISR(EHmiEvent ev);
	SCBoard* GetBoard(){return(m_screen);}
	
private :
	void run(void);
	void main(void);
	void eventHandler(EHmiEvent& ev);
	void startScreen(void);
	void changeScreen(const short id, const EHmiEvent& ev);

private :
	EventGroupHandle_t	ev_flag;
	TimerHandle_t		timer;
	QueueHandle_t		mtx;
	QueueHandle_t		deq;
	bool				is_ready;
	short		   		m_screen_id;
	SCBoard*			m_screen;
};

#endif // __EHMI_MAIN_H__
