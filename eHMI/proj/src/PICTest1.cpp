///
/// file		PICTest1.cpp
/// brief		test1 screen class file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/12/24
///
#include "SCLabel.h"
#include "PICTest1.h"

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
	SCRect		area(10, 30, 300, 80);
	SCLabel* 	label = new SCLabel(1, area, COLOR(Red));

	label->setStr("Welcome to Test1 screen!");
	RegistParts(static_cast<SCParts*>(label));
}

