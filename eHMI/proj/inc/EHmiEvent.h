///
/// file		EHmiEvent.h
/// brief		HMI main process file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/11/26
///

#ifndef __EHMI_EVENT_H__
#define __EHMI_EVENT_H__

typedef union _EHmiEventParam{
	unsigned long	lp;
	unsigned int    ip[2];
	unsigned short  sp[4];
	
	_EHmiEventParam() {}
	_EHmiEventParam(unsigned long ul) {lp = ul;}
	_EHmiEventParam(unsigned int i1, unsigned int i2) {ip[0] = i1; ip[1] = i2;}
} EHmiEventParam;

typedef enum {
    HMI_EV_NONE,
	HMI_EV_INIT,
    HMI_EV_MOUSE_DOWN,
    HMI_EV_MOUSE_UP,
} EHmiEventType;

///
/// class : EHmiEvent
/// HMI event class
///
class EHmiEvent {
public :
    EHmiEvent();
    EHmiEvent(EHmiEventType ev);
    EHmiEvent(EHmiEventType ev, unsigned long lp);
    EHmiEvent(EHmiEventType ev, 
			unsigned int ip1,
			unsigned int ip2
			);
	/*
    EHmiEvent(EHmiEventType ev,
			unsigned short sp1,
			unsigned short sp2,
			unsigned short sp3,
			unsigned short sp4
			);
	*/
    ~EHmiEvent();
    EHmiEventType GetEvent() {return(type);}
    unsigned long GetParam() {return(arg.lp);}
    void GetParam(unsigned int* i1, unsigned int* i2) {
			*i1 = arg.ip[0];
			*i2 = arg.ip[1];
	}
	void SetParam(unsigned long ul) {arg.lp = ul;}
	void SetParam(unsigned int i1, unsigned int i2) {
			arg.ip[0] = i1;
			arg.ip[1] = i2;
	}
	
private :
    EHmiEventType	type;
    EHmiEventParam	arg;
};

#endif // __EHMI_EVENT_H__
