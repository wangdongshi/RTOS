/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename:  PICTest2.cpp
 * Project:   Minimum RTOS platform
 * Date:	  2018/11/26
 * Author:	  WangYu
 *
 **********************************************************************/
#include "SCLabel.h"
#include "SCTimeLabel.h"
#include "SCButton.h"
#include "PICTest2.h"

const SCRect PICTest2::area_top_label(80, 60, 320, 20);
const SCRect PICTest2::area_time_label(80, 100, 320, 50);
const SCRect PICTest2::area_button(150, 200, 180, 50);

static void cbButtonPushed(const int id)
{
	SCBoard* screen = static_cast<SCBoard*>(pHmi->GetBoard());
    static_cast<PICTest2*>(screen)->ButtonPushed(id);
}

PICTest2::PICTest2(const SCRect& area, const short id) :
SCBoard(area, id)
{
	InitScreen();
}

PICTest2::~PICTest2()
{
}

void PICTest2::InitScreen(void)
{
	SCLabel*		label		= new SCLabel(1, area_top_label, SC_COLOR_BLUE);
	SCTimeLabel*	moment		= new SCTimeLabel(2, area_time_label, SC_COLOR_RED, SC_COLOR_DARK_GRAY);
	SCButton* 		button		= new SCButton(3, area_button);
	SCCallback*		callback	= new SCCallback(SCCallbackTypeTAP, cbButtonPushed, 0);

	// top label
	label->SetStr("Welcome to Test2 screen!");
	RegistParts(static_cast<SCParts*>(label));

	// time label
	RegistParts(static_cast<SCParts*>(moment));
	
	// bottom button
	button->SetStr("Back");
	button->AddCallback(callback); // add button push callback
	RegistParts(static_cast<SCButton*>(button));
}

void PICTest2::ButtonPushed(const int id)
{
	EHmiEvent hmi_ev(HMI_EV_SCREEN_CHG, SCREEN_TEST1);
	xSemaphoreTake(pHmi->Mutex(), 0);
	pHmi->SendQueue(hmi_ev);
	xSemaphoreGive(pHmi->Mutex());
}
