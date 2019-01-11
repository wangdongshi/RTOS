///
/// file		SCTimeLabel.h
/// brief		HMI date and clock label class define head file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2019/01/11
///

#ifndef __SCL_TIME_LABEL_H__
#define __SCL_TIME_LABEL_H__

#include "SCLabel.h"

///
/// class : SCTimeLabel
/// Label class
///
class SCTimeLabel : public SCLabel {
public :
    SCTimeLabel(const short id,
			const SCRect& area,
			const XColor& fore_color = COLOR(Black),
			const XColor& back_color = COLOR(Transparent),
			const std::string& font = SC_FONT_MIDDLE);
	virtual ~SCTimeLabel();

	virtual bool	Update(void);
	void			SetStr(const char* mes){}			// Prohibit setting string
	void			SetStr(const unsigned short* mes){}	// Prohibit setting string
};

#endif // __SCL_TIME_LABEL_H__

