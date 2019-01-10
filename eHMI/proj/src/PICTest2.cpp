///
/// file		PICTest2.cpp
/// brief		test2 screen class file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2019/01/10
///
#include "EHmiCtrl.h"
#include "SCLabel.h"
#include "SCButton.h"
#include "PICTest2.h"

const SCRect PICTest2::area_label(10, 30, 300, 80);
const SCRect PICTest2::area_button(130, 150, 60, 30);

/// function	ButtonPushed
/// brief		button callback
///
/// param		id		part's ID
/// return		none
static void cbButtonPushed(const int id)
{
    SCBoard* screen = (SCBoard*)EHMI->GetBoard();
    static_cast<PICTest2*>(screen)->ButtonPushed(id);
}

/// function	PICTest2
/// brief		constructor
PICTest2::PICTest2(const SCRect& area, const short id) :
SCBoard(area, id)
{
	InitScreen();
}

/// function	~PICTest2
/// brief		deconstructor
PICTest2::~PICTest2()
{
}

/// function	InitScreen
/// brief		initialize test2 screen
///
/// param		none
/// return		none
void PICTest2::InitScreen(void)
{
	SCLabel* 	label		= new SCLabel(1, area_label, COLOR(Blue));
	SCButton* 	button		= new SCButton(2, area_button);
	SCCallback*	callback	= new SCCallback(SCCallbackTypeTAP, cbButtonPushed, 0);

	// top label
	label->SetStr("Welcome to Test2 screen!");
	RegistParts(static_cast<SCParts*>(label));
	
	// bottom button
	button->SetStr("Back");
	button->AddCallback(callback); // add button push callback
	RegistParts(static_cast<SCButton*>(button));
}

/// function	ButtonPushed
/// brief		callback function when button is pushed
///
/// param		id		part's ID
/// return		none
void PICTest2::ButtonPushed(const int id)
{
	EHmiEvent hmi_ev(HMI_EV_CHG_SCREEN, SCREEN_TEST1);
	SendMassage(hmi_ev);
}
