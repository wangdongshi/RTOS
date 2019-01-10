///
/// file		SCBoard.h
/// brief		HMI board class define head file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2018/12/20
///

#ifndef __SCL_BOARD_H__
#define __SCL_BOARD_H__

#include "SCBase.h"
#include "SCParts.h"

class SCParts;

///
/// class : SCBoard
/// Board class
///
class SCBoard : public SCCore {
public :
	SCBoard(const SCRect& area, const short id = -1); // -1: illegal ID
	virtual ~SCBoard();

	void			RegistParts(SCParts* target);
	void			UnregistParts(SCParts* target);
	void			DestroyParts(SCParts* target);
	void			DestroyAllParts(void);
	
	void			Initialize(void);
	virtual bool	Draw(void);
	//virtual int		Update(const int updateType, unsigned short* com);
	
	virtual bool	DrawBackground(void);
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
