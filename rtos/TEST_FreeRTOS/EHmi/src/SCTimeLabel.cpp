/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename:  SCTimeLabel.cpp
 * Project:   Minimum RTOS platform
 * Date:	  2018/11/26
 * Author:	  WangYu
 *
 **********************************************************************/
#include <time.h>
#include "SCTimeLabel.h"

SCTimeLabel::SCTimeLabel(const short id,
		const SCRect& area,
		const unsigned int fore_color,
		const unsigned int back_color,
		const unsigned int font) :
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

SCTimeLabel::~SCTimeLabel()
{
}

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
