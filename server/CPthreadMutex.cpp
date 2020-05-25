#include "CPthreadMutex.h"
#include <errno.h>

CPthreadMutex::CPthreadMutex()
{
	if(pthread_mutex_init(&m_mutex,NULL) != 0)
	{
		perror("pthread_mutex_init error");
	}
}

CPthreadMutex::~CPthreadMutex()
{
	if(pthread_mutex_destroy(&m_mutex) != 0)
	{
		perror("pthread_mutex_destroy error");
	}
}

bool CPthreadMutex::lock()
{
	if(pthread_mutex_lock(&m_mutex) != 0)
	{
		perror("pthread_mutex_lock error");
		return false;
	}
	return true;
}

bool CPthreadMutex::unlock()
{
	if(pthread_mutex_unlock(&m_mutex) != 0)
	{
		perror("pthread_mutex_unlock error");
		return false;
	}
	return true;
}
	
bool CPthreadMutex::trylock()
{
	if(pthread_mutex_trylock(&m_mutex) != 0)
	{
		perror("pthread_mutex_trylock error");
		return false;
	}
	return true;
}
