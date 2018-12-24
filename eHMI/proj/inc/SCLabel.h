///
/// file		SCParts.h
/// brief		HMI parts class define head file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/12/20
///

#ifndef __SCL_LABEL_H__
#define __SCL_LABEL_H__

#include "SCParts.h"
#include "SCDrawCommand.h"

#define SC_LABEL_STRING_MAX     (60)

///
/// class : SCLabel
/// Parts class
///
class SCLabel : public SCParts {
public :
    SCLabel(const short id,
			const SCRect& area,
			const XColor& fore_color = COLOR(Black),
			const XColor& back_color = COLOR(Transparent),
			const std::string& font = SC_FONT_MIDDLE);
	virtual ~SCLabel();

	virtual bool	Draw(void);
	void			setStr(const char* mes);
	void			setStr(const unsigned short* mes);
	void			Arrangement(int arr){this->m_arrangement = arr;}
	
private:
	std::string		m_font;
	unsigned short	m_label[SC_LABEL_STRING_MAX+1];
	int				m_arrangement;
};

#endif // __SCL_LABEL_H__

