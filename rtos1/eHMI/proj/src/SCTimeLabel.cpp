///
/// file		SCTimeLabel.cpp
/// brief		HMI date and clock label class file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2019/01/11
///
#include "debug.h"
#include "SCTimeLabel.h"

/// function	SCTimeLabel
/// brief		constructor
SCTimeLabel::SCTimeLabel(const short id,
		const SCRect& area,
		const XColor& fore_color,
		const XColor& back_color,
		const std::string& font) :
SCLabel(id, area, fore_color, back_color, font)
{
	time_t tt = GetTime();
	tm* t= gmtime(&tt);
	char sys_time[SC_LABEL_STRING_MAX+1] = "";
	
	sprintf(sys_time, "%d-%02d-%02d %02d:%02d:%02d",
			t->tm_year + 1900,
			t->tm_mon + 1,
			t->tm_mday,
			t->tm_hour,
			t->tm_min,
			t->tm_sec);
	SCLabel::SetStr(sys_time);
}

/// function	~SCTimeLabel
/// brief		deconstructor
SCTimeLabel::~SCTimeLabel()
{
}

/// function	Draw
/// brief		draw label part
///
/// param		none
/// return		success of failed
bool SCTimeLabel::Update(void)
{
	time_t tt = GetTime();
	tm* t= gmtime(&tt);
	char sys_time[SC_LABEL_STRING_MAX+1] = "";
	
	sprintf(sys_time, "%d-%02d-%02d %02d:%02d:%02d",
			t->tm_year + 1900,
			t->tm_mon + 1,
			t->tm_mday,
			t->tm_hour,
			t->tm_min,
			t->tm_sec);
	SCLabel::SetStr(sys_time);
	
	return SCParts::Update();
}
