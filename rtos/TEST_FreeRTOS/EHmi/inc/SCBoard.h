/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename : SCBoard.h
 * Project  : Minimum RTOS platform
 * Date     : 2018/12/20
 * Author   : WangYu
 *
 **********************************************************************/
#ifndef __SCL_BOARD_H__
#define __SCL_BOARD_H__

#include "SCBase.h"
#include "SCParts.h"

class SCParts;

class SCBoard : public SCCore {
public :
	SCBoard(const SCRect& area, const short id = -1); // -1: illegal ID
	virtual ~SCBoard();

	void			RegistParts(SCParts* target);
	void			UnregistParts(SCParts* target);
	void			DestroyParts(SCParts* target);
	void			DestroyAllParts(void);
	
	void			Initialize(void);
	virtual bool	DrawBackground(void);
	virtual bool	Draw(void);
	virtual bool	Update(void);
	
	virtual void	TDown(const SCPoint& point);
	virtual void	TUp(const SCPoint& point);
	virtual void	TMove(const SCPoint& point);
	SCParts*		GetChild(void) const {return(m_child);}
	SCParts*		GetChild(const unsigned short x, const unsigned short y) const;
	SCParts*		GetChild(const SCPoint& point) const;
	SCParts*		GetChild(const short id) const;
	SCParts*		GetLastChild(void) const;
	SCParts*		GetOperableChild(const SCPoint& point) const;

	short			GetScreenID() {return(m_screen_id);}

protected :
	short			m_screen_id;	// screen ID
	SCParts*		m_child;		// child parts
	SCParts*		m_captured;		// part on touch event
	//SCBoard*		m_dialog;		// dialog
	//SCBoard*		m_parent;		// father dialog
	SCPoint			m_touch_point;	// touch point (for Callback)
};

#endif // __SCL_BOARD_H__
