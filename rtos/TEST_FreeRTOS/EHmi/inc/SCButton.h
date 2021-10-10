/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : SCButton.h
 * Project  : Minimum RTOS platform
 * Date     : 2018/11/26
 * Author   : WangYu
 *
 **********************************************************************/
#ifndef __SCL_BUTTON_H__
#define __SCL_BUTTON_H__

#include "SCLabel.h"

class SCButton : public SCLabel {
public :
	SCButton(const short id,
			const SCRect& area,
			const unsigned int fore_color = SC_COLOR_BLACK,
			const unsigned int back_color = SC_COLOR_LIGHT_GRAY,
			const unsigned int font = SC_FONT_MIDDLE);
	virtual ~SCButton();

	virtual bool	Draw(void);
	virtual bool	DrawBackground(void);
	virtual bool	Update(void){return true;}
	virtual void	TDown(const SCPoint& point);
	virtual void	TUp(const SCPoint& point);
	virtual void	TMove(const SCPoint& point);
	
protected:
	virtual unsigned int	getButtonColor(void);
	virtual bool			drawUnderLine(void);
	virtual bool			drawRightLine(void);
};

#endif // __SCL_BUTTON_H__

