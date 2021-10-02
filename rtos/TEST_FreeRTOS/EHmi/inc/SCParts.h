/**********************************************************************
 * Copyright (c) 2018 - 2021 by WangYu
 * All rights reserved
 *
 * Filename:  SCParts.h
 * Project:   Minimum RTOS platform
 * Date:      2018/12/20
 * Author:    WangYu
 *
 **********************************************************************/
#ifndef __SCL_PARTS_H__
#define __SCL_PARTS_H__

#include "SCCore.h"
#include "SCBase.h"
#include "SCBoard.h"

class SCBoard;

class SCParts : public SCCore {
public :
	SCParts(const short id, const SCRect& area,
			const uint32_t fore_color = SC_COLOR_BLACK,
			const uint32_t back_color = SC_COLOR_TRANS);
	virtual ~SCParts();

	virtual bool	Draw(void) = 0;
	virtual bool	DrawBackground(void);
	//virtual int		Update(const int updateType, unsigned short* para);
	//virtual void	TDown(const SCPoint& point);
	//virtual void	TUp(const SCPoint& point);
	//void			AddCallback(SCCallback* cb);
	//void			DoCallback(int type);
	virtual void	Initialize() {}

	SCBoard*		Parent(void) const {return(m_parent);}
	void			Parent(SCBoard* parent) {m_parent = parent;}
	SCParts*		Next(void) const {return(m_next);}
	void			Next(SCParts* next) {m_next = next;}
	SCParts*		Prev(void) const {return(m_prev);}
	void			Prev(SCParts* prev) {m_prev = prev;}
	short			GetID() const {return(m_id);}

protected :
	short			m_id;			// part ID
	SCBoard*		m_parent;		// father screen
	SCParts*		m_next;			// next part by register sequence
	SCParts*		m_prev;			// previous part by register sequence
	//SCCallback*	m_callback;		// call back list pointer
	//bool			m_fVisible;		// visible?
	//bool			m_fEnable;		// selectable?
	//bool			m_fPushed;		// in push state?
	uint32_t		m_fore_color;	// foreground color
	uint32_t		m_back_color;	// background color
};

#endif // __SCL_PARTS_H__
