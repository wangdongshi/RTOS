/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename:  SCLabel.cpp
 * Project:   Minimum RTOS platform
 * Date:      2018/12/20
 * Author:    WangYu
 *
 **********************************************************************/
#include <string.h>
#include "SCLabel.h"

SCLabel::SCLabel(const short id,
		const SCRect& area,
		const unsigned int fore_color,
		const unsigned int back_color,
		const unsigned int font) :
SCParts(id, area, fore_color, back_color),
m_font(font),
m_arrangement(ARRANGE_MIDDLE)
{
	memset(m_label, 0x0000, sizeof(m_label));
}

SCLabel::~SCLabel()
{
}

bool SCLabel::Draw(void)
{
	bool res = true;

	unsigned int x = 0;
	unsigned int y = m_area.y + m_area.height / 2 - 7;

	res &= DrawBackground();

	unsigned int str_width = GetStrWidth(m_label, m_font);
	if (m_arrangement == ARRANGE_MIDDLE) {
		x = m_area.x + m_area.width / 2 - str_width / 2;
	} else if (m_arrangement == ARRANGE_LEFT) {
		x = m_area.x + 2;
	} else if (m_arrangement == ARRANGE_RIGHT) {
		x = m_area.x + m_area.width - 2 - str_width;
	}
	
	res &= MakeString(x, y, m_label, m_fore_color, m_back_color, m_font);
	return res;
}

void SCLabel::SetStr(const char* mes)
{
	int i = 0;
	while (mes[i] != '\0' && i < SC_LABEL_STRING_MAX)
	{
		m_label[i] = static_cast<unsigned short>(mes[i]);
		++i;
	}
	m_label[i] = 0x0000;
}

void SCLabel::SetStr(const unsigned short* mes)
{
	int i = 0;
	while (mes[i] != 0x0000 && i < SC_LABEL_STRING_MAX)
	{
		m_label[i] = mes[i];
		++i;
	}
	m_label[i] = 0x0000;
}

bool SCLabel::drawString(void)
{
	unsigned int x = 0;
	unsigned int y = m_area.y + m_area.height / 2 - 7;

	unsigned int str_width = GetStrWidth(m_label, m_font);
	if (m_arrangement == ARRANGE_MIDDLE) {
		x = m_area.x + m_area.width / 2 - str_width / 2;
	} else if (m_arrangement == ARRANGE_LEFT) {
		x = m_area.x + 2;
	} else if (m_arrangement == ARRANGE_RIGHT) {
		x = m_area.x + m_area.width - 2 - str_width;
	}

	return MakeString(x, y, m_label, m_fore_color, SC_COLOR_TRANS, m_font);
}
