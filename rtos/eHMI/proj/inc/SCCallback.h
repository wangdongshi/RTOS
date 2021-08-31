///
/// file		SCCallback.h
/// brief		HMI call back function class define head file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2019/01/03
///

#ifndef __SCL_CALLBACK_H__
#define __SCL_CALLBACK_H__

// call back type
enum SCCallbackType {
	SCCallbackTypeTAP = 0,		// tap from touch pannel
	SCCallbackTypeFLICK,		// flick from touch pannel
	SCCallbackTypeLONGTAP,		// long tap from touch pannel
	SCCallbackTypeCHANGEVALUE	// value change(if not to be used, delete it)
};

///
/// class : SCCallback
/// Call back class
///
class SCCallback {
public :
	SCCallback(const int type, void (*func)(int), const int data);
	virtual ~SCCallback();

	void			DoCallback(void);

	SCCallback*		Next(void) const {return(m_next);}
	void			Next(SCCallback* next) {m_next = next;}

	int				GetType(void) const {return(m_type);}
	
private :
	void			(*m_func)(const int data);
	int				m_type;		// call back type
	int				m_data;		// parameter in call back function
	SCCallback*		m_next;		// next call back function point by registered sequence
};

#endif // __SCL_CALLBACK_H__

