/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : SCTimeLabel.h
 * Project  : Minimum RTOS platform
 * Date     : 2018/11/26
 * Author   : WangYu
 *
 **********************************************************************/
#ifndef __SCL_TIME_LABEL_H__
#define __SCL_TIME_LABEL_H__

#include "SCLabel.h"

class SCTimeLabel : public SCLabel {
public :
	SCTimeLabel(const short id,
			const SCRect& area,
			const unsigned int fore_color = SC_COLOR_BLACK,
			const unsigned int back_color = SC_COLOR_TRANS,
			const unsigned int font = SC_FONT_MIDDLE);
	virtual ~SCTimeLabel();

	virtual bool	Update(void);
	void			SetStr(const char* mes){}			// Prohibit setting string
	void			SetStr(const unsigned short* mes){}	// Prohibit setting string
};

#endif // __SCL_TIME_LABEL_H__
