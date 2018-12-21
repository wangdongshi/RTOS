///
/// file		EHmiCtrl.h
/// brief		HMI touch event monitor process file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/12/07
///
#include <X11/Xlib.h>
#include "EHmiEvent.h"
#include "EHmiMain.h"

#ifndef __EHMI_CTRL_H__
#define __EHMI_CTRL_H__

///
/// class : EHmiCtrl
/// HMI contrl process class
///
class EHmiCtrl {
public :
    EHmiCtrl();
    EHmiCtrl(EHmiMain* pHmi);
    virtual ~EHmiCtrl();
	
public :
	void Start(void) {main();}
	
private :
	void main(void);
	void send2HMI(EHmiEvent& ev);

private :
	EHmiMain*				hmi;
};

#endif // __EHMI_CTRL_H__
