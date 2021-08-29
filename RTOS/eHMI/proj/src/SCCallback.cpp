///
/// file		SCCallback.cpp
/// brief		HMI call back function class file
///
/// author		Wang.Yu
/// version 	00.01.00
/// date		2019/01/03
///

#include "SCCallback.h"

/// function	SCCallback
/// brief		constructor
SCCallback::SCCallback(const int type, void	(*func)(int), const int	data) :
m_func(func),
m_type(type),
m_data(data),
m_next(0)
{
}

/// function	~SCCallback
/// brief		deconstructor
SCCallback::~SCCallback(
)
{
}

/// function	DoCallback
/// brief		execute call back function
///
/// param		none
/// return		none
void SCCallback::DoCallback(void)
{
	if (m_func) m_func(m_data);
}
