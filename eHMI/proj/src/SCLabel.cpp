///
/// file		SCLabel.cpp
/// brief		HMI label class file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/12/24
///
#include "SCLabel.h"

/// function	SCLabel
/// brief		constructor
SCLabel::SCLabel(const short id,
		const SCRect& area,
		const XColor& fore_color,
		const XColor& back_color,
		const std::string& font) :
SCParts(id, area, fore_color, back_color),
m_font(font),
m_arrangement(ARRANGE_MIDDLE)
{
	memset(m_label, 0x0000, sizeof(m_label));
}

/// function	~SCLabel
/// brief		deconstructor
SCLabel::~SCLabel()
{
}

/// function	Draw
/// brief		draw label part
///
/// param		none
/// return		success of failed
bool SCLabel::Draw(void)
{
	bool res = true;

	unsigned int x;
	unsigned int y = m_area.y + m_area.height / 2 - 7;

	res &= DrawBackground();

	unsigned int str_width = GetStrWidth(m_label);
	if (m_arrangement == ARRANGE_MIDDLE) {
		x = m_area.x + m_area.width / 2 - str_width / 2;
	} else if (m_arrangement == ARRANGE_LEFT) {
		x = m_area.x + 2;
	} else if (m_arrangement == ARRANGE_RIGHT) {
		x = m_area.x + m_area.width - 2 - str_width;
	}
	
	res &= MakeString(x, y, m_label, m_fore_color, m_back_color, SC_FONT_MIDDLE);
	return res;
}

/// function	setStr
/// brief		set string for label part
///
/// param		mes		string's head address
/// return		none
void SCLabel::setStr(const char* mes)
{
	int i = 0;
	while (mes[i] != '\0' && i < SC_LABEL_STRING_MAX)
	{
		m_label[i] = static_cast<unsigned short>(mes[i]);
		++i;
	}
	m_label[i] = 0x0000;
}

/// function	setStr
/// brief		set string for label part
///
/// param		mes		string's head address
/// return		none
void SCLabel::setStr(const unsigned short* mes)
{
	int i = 0;
	while (mes[i] != 0x0000 && i < SC_LABEL_STRING_MAX)
	{
		m_label[i] = mes[i];
		++i;
	}
	m_label[i] = 0x0000;
}

