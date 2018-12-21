///
/// file		SCParts.h
/// brief		HMI parts class define head file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/12/20
///

#ifndef __SCL_PARTS_H__
#define __SCL_PARTS_H__

#include "SCCore.h"
#include "SCColor.h"
#include "SCBase.h"
#include "SCBoard.h"

///
/// class : SCParts
/// Parts class
///
class SCParts : public SCCore {
public :
	SCParts(const short id, const SCRect& area,
			const XColor fore_color = SC_COLOR("Black"),
			const XColor back_color = SC_COLOR("LightGray"));
	virtual ~SCParts() = 0;

	virtual void	Initialize() = 0;
	virtual bool	Draw(void) = 0;
	virtual bool	DrawBackground(void);
	//virtual int		Update(const int updateType, unsigned short* para);
	//virtual void	TDown(const SCPoint& point);
	//virtual void	TUp(const SCPoint& point);
	//void			AddCallback(SCCallback* cb);
	//void			DoCallback(int type);

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
	//SCCallback*		m_callback;		// call back list pointer
	//bool			m_fVisible;		// visible?
	//bool			m_fEnable;		// selectable?
	//bool			m_fPushed;		// in push state?
	XColor			m_fore_color;	// foreground color
	XColor			m_back_color;	// background color
};

#endif // __SCL_PARTS_H__
