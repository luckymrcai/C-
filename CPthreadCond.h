#ifndef _CPTHREADCOND_H_
#define _CPTHREADCOND_H_

#include "CPthreadMutex.h"

class CPthreadCond:public CPthreadMutex
{
public:
	CPthreadCond();
	~CPthreadCond();
	bool wait();
	bool timewait(unsigned int sec);
	bool signal();
	bool broadcast();
	bool isTimeout();

private:
	pthread_cond_t m_cond;
	bool m_btimeout;
	
};


#endif