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

#define	EHMI			EHmiCtrl::GetHMI()
#define	SendMassage		EHmiCtrl::Send2HMI


///
/// class : EHmiCtrl
/// HMI control process class
///
class EHmiCtrl {
public :
    EHmiCtrl();
    virtual ~EHmiCtrl();
	
public :
	static void				Send2HMI(EHmiEvent& ev);
	static EHmiMain*		GetHMI(void){return(hmi);}
	static void				SetHMI(EHmiMain* hmi_main){hmi = hmi_main;}
	void					Start(void) {main();}
	
private :
	void					main(void);

private :
	static EHmiMain*		hmi;
	unsigned int			move_cnt;
};

#endif // __EHMI_CTRL_H__
