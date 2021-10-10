/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : SCLabel.h
 * Project  : Minimum RTOS platform
 * Date     : 2018/12/20
 * Author   : WangYu
 *
 **********************************************************************/
#ifndef __SCL_LABEL_H__
#define __SCL_LABEL_H__

#include "SCParts.h"
#include "SCDrawCommand.h"

#define SC_LABEL_STRING_MAX	 (60)

class SCLabel : public SCParts {
public :
	SCLabel(const short id,
			const SCRect& area,
			const unsigned int fore_color = SC_COLOR_BLACK,
			const unsigned int back_color = SC_COLOR_TRANS,
			const unsigned int font = SC_FONT_MIDDLE);
	virtual ~SCLabel();

	virtual bool	Draw(void);
	virtual bool	Update(void){return true;}
	void			SetStr(const char* mes);
	void			SetStr(const unsigned short* mes);
	void			Arrangement(int arr){this->m_arrangement = arr;}

protected :
	virtual bool	drawString(void);
	
protected:
	unsigned int	m_font;
	unsigned short	m_label[SC_LABEL_STRING_MAX+1];
	int				m_arrangement;
};

#endif // __SCL_LABEL_H__

