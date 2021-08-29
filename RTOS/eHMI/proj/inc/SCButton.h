///
/// file		SCButton.h
/// brief		HMI button class define head file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2019/01/03
///

#ifndef __SCL_BUTTON_H__
#define __SCL_BUTTON_H__

#include "SCLabel.h"

///
/// class : SCButton
/// Button class
///
class SCButton : public SCLabel {
public :
    SCButton(const short id,
			const SCRect& area,
			const XColor& fore_color = COLOR(Black),
			const XColor& back_color = COLOR(LightGray),
			const std::string& font = SC_FONT_MIDDLE);
    virtual ~SCButton();

	virtual bool	Draw(void);
	virtual bool	DrawBackground(void);
	virtual bool	Update(void){return true;}
    virtual void    TDown(const SCPoint& point);
    virtual void	TUp(const SCPoint& point);
    virtual void	TMove(const SCPoint& point);
	
protected:
    virtual XColor getButtonColor(void);
    virtual bool drawUnderLine(void);
	virtual bool drawRightLine(void);
};

#endif // __SCL_BUTTON_H__

