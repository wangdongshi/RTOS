///
/// file		SCCore.cpp
/// brief		HMI core class source file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/12/19
///

#include "SCCore.h"

using namespace std;

/// function	SCCore
/// brief		constructor
SCCore::SCCore(const SCRect& rect) :
m_area(rect)
{
}

/// function	SCCore
/// brief		constructor
SCCore::SCCore(
	const unsigned short x,
	const unsigned short y,
	const unsigned short width,
	const unsigned short height) :
m_area(x, y, width, height)
{
}

/// function	~SCCore
/// brief		deconstructor
SCCore::~SCCore()
{
}

/// function	GetArea
/// brief		get part's rect area
///
/// param		none
/// return		area of part
SCRect SCCore::GetArea(void) const
{
	return m_area;
}

