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
#include "PICTest1.h"

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
	SCRect*  area;
	SCLabel* label;

	area	= new SCRect(50, 50, 380, 132);
	label	= new SCLabel(1, *area, SC_COLOR_TRANS, SC_COLOR_ROYAL_BLUE);
	RegistParts(static_cast<SCParts*>(label));

	area	= new SCRect(50, 60, 380, 20);
	label	= new SCLabel(2, *area, SC_COLOR_YELLOW);
	label->setStr("Welcome to STM32F746G-DISCO!");
	label->Arrangement(ARRANGE_MIDDLE);
	RegistParts(static_cast<SCParts*>(label));

	area	= new SCRect(280, 120, 140, 20);
	label	= new SCLabel(3, *area, SC_COLOR_YELLOW, SC_COLOR_TRANS, SC_FONT_SMALL);
	label->setStr("Author : Wang.Yu");
	label->Arrangement(ARRANGE_RIGHT);
	RegistParts(static_cast<SCParts*>(label));

	area	= new SCRect(280, 140, 140, 20);
	label	= new SCLabel(4, *area, SC_COLOR_YELLOW, SC_COLOR_TRANS, SC_FONT_SMALL);
	label->setStr("Date : 2021/9/1");
	label->Arrangement(ARRANGE_RIGHT);
	RegistParts(static_cast<SCParts*>(label));
}
