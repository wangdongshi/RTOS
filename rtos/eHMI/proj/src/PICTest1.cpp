///
/// file		PICTest1.cpp
/// brief		test1 screen class file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/12/24
///
#include "EHmiCtrl.h"
#include "SCLabel.h"
#include "SCButton.h"
#include "PICTest1.h"

const SCRect PICTest1::area_label(10, 30, 300, 80);
const SCRect PICTest1::area_button(130, 150, 60, 30);

/// function	ButtonPushed
/// brief		button callback
///
/// param		id		part's ID
/// return		none
static void cbButtonPushed(const int id)
{
    SCBoard* screen = (SCBoard*)EHMI->GetBoard();
    static_cast<PICTest1*>(screen)->ButtonPushed(id);
}

/// function	PICTest1
/// brief		constructor
PICTest1::PICTest1(const SCRect& area, const short id) :
SCBoard(area, id)
{
	InitScreen();
}

/// function	~PICTest1
/// brief		deconstructor
PICTest1::~PICTest1()
{
}

/// function	InitScreen
/// brief		initialize test1 screen
///
/// param		none
/// return		none
void PICTest1::InitScreen(void)
{
	SCLabel* 	label		= new SCLabel(1, area_label, COLOR(Red));
	SCButton* 	button		= new SCButton(2, area_button);
	SCCallback*	callback	= new SCCallback(SCCallbackTypeTAP, cbButtonPushed, 0);

	// top label
	label->SetStr("Welcome to Test1 screen!");
	RegistParts(static_cast<SCParts*>(label));
	
	// bottom button
	button->SetStr("Next");
	button->AddCallback(callback); // add button push callback
	RegistParts(static_cast<SCButton*>(button));
}

/// function	ButtonPushed
/// brief		callback function when button is pushed
///
/// param		id		part's ID
/// return		none
void PICTest1::ButtonPushed(const int id)
{
	EHmiEvent hmi_ev(HMI_EV_CHG_SCREEN, SCREEN_TEST2);
	SendMassage(hmi_ev);
}
