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

///
/// class : SCBoard
/// Board class
///
class SCBoard : public SCCore {
public :
	SCBoard(const SCRect& area, const short sID = -1); // -1: illegal ID
	virtual ~SCBoard();

	virtual int		Draw(unsigned short* com);
	virtual int		Update(const int updateType, unsigned short* com);
	
	virtual int		DrawBackground(const SCRect& area, unsigned short* com);
	virtual void	TDown(const unsigned short x, const unsigned short y);
	virtual void	TUp(const unsigned short x, const unsigned short y);
	SCParts*		GetChild(void) const {return(m_child);}
	SCParts*		GetChild(const unsigned short x, const unsigned short y) const;
	SCParts*		GetChild(const SCPoint& point) const;
	SCParts*		GetChild(const short id) const;
	void			RegistParts(SCParts* p);
	void			DestroyParts(SCParts* p);
	void			DestroyAllParts(void);

	short			GetScreenID() {return(m_screenID);}

protected :
	short			m_screenID;		// screen ID
	SCParts*		m_child;		// child parts
	SCParts*		m_captured;		// part on touch event
	//SCBoard*		m_dialog;		// dialog
	//SCBoard*		m_parent;		// father dialog
	SCPoint			m_touch_point;	// touch point (for Callback)
};

#endif // __SCL_BOARD_H__
