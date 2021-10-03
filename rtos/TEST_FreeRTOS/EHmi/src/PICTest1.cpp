/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename:  PICTest.cpp
 * Project:   Minimum RTOS platform
 * Date:      2018/12/20
 * Author:    WangYu
 *
 **********************************************************************/
#include "SCLabel.h"
#include "SCButton.h"
#include "SCCallback.h"
#include "PICTest1.h"

const SCRect PICTest1::area_rect(50, 50, 380, 120);
const SCRect PICTest1::area_label1(50, 60, 380, 20);
const SCRect PICTest1::area_label2(280, 120, 140, 20);
const SCRect PICTest1::area_label3(280, 140, 140, 20);
const SCRect PICTest1::area_button(150, 200, 180, 50);

static void cbButtonPushed(const int id)
{
    SCBoard* screen = static_cast<SCBoard*>(pHmi->GetBoard());
    static_cast<PICTest1*>(screen)->ButtonPushed(id);
}

PICTest1::PICTest1(const SCRect& area, const short id) :
SCBoard(area, id)
{
	InitScreen();
}

PICTest1::~PICTest1()
{
}

void PICTest1::InitScreen(void)
{
	SCLabel*	label;
	SCButton*	button;
	SCCallback*	callback;

	label = new SCLabel(1, area_rect, SC_COLOR_TRANS, SC_COLOR_ROYAL_BLUE);
	RegistParts(static_cast<SCParts*>(label));

	label = new SCLabel(2, area_label1, SC_COLOR_YELLOW);
	label->SetStr("Welcome to STM32F746G-DISCO!");
	label->Arrangement(ARRANGE_MIDDLE);
	RegistParts(static_cast<SCParts*>(label));

	label = new SCLabel(3, area_label2, SC_COLOR_YELLOW, SC_COLOR_TRANS, SC_FONT_SMALL);
	label->SetStr("Author : Wang.Yu");
	label->Arrangement(ARRANGE_RIGHT);
	RegistParts(static_cast<SCParts*>(label));

	label = new SCLabel(4, area_label3, SC_COLOR_YELLOW, SC_COLOR_TRANS, SC_FONT_SMALL);
	label->SetStr("Date : 2021/9/1");
	label->Arrangement(ARRANGE_RIGHT);
	RegistParts(static_cast<SCParts*>(label));

	button = new SCButton(2, area_button);
	callback = new SCCallback(SCCallbackTypeTAP, cbButtonPushed, 0);
	button->SetStr("Next");
	button->AddCallback(callback);
	RegistParts(static_cast<SCButton*>(button));
}

void PICTest1::ButtonPushed(const int id)
{
	EHmiEvent hmi_ev(HMI_EV_SCREEN_CHG, SCREEN_TEST2);
	xSemaphoreTake(pHmi->Mutex(), 0);
	pHmi->SendQueue(hmi_ev);
	xSemaphoreGive(pHmi->Mutex());
}
