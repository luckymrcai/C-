#ifndef _CPTHREADMUTEX_H_
#define _CPTHREADMUTEX_H_

#include <stdio.h>
#include <pthread.h>

class CPthreadMutex
{
public:
	CPthreadMutex();
	~CPthreadMutex();
	bool lock();
	bool unlock();
	bool trylock();
protected:
	pthread_mutex_t m_mutex;
	
};




#endif