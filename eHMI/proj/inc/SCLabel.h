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

#include "SCPart.h"

#define SC_LABEL_STRING_MAX     (60)

///
/// class : SCLabel
/// Parts class
///
class SCLabel : public SCParts {
public :
    SCLabel(const short id,
			const SCRect& area,
			const std::string& font = SC_FONT_MIDDLE,
			const XColor& fore_color = SC_COLOR("Black"),
			const XColor& back_color = SC_COLOR("LightGray"))
	virtual ~SCLabel();

	virtual int		Draw(unsigned short* com);
	void			setStr(const char* mes);
	void			setStr(const unsigned short* mes);
	void			setStr(int target_id);
	void			Arrangement(int arr){this->m_arrangement = arr;};
	
private:
	unsigned short	m_font;
	unsigned short	m_label[SC_LABEL_STRING_MAX+1];
	int				m_targetID;
	int				m_arrangement;
};

#endif // __SCL_LABEL_H__

