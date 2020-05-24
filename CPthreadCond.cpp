#include "CPthreadCond.h"
#include <sys/time.h>
#include <errno.h>
#include <time.h>

CPthreadCond::CPthreadCond()
{
	if(pthread_cond_init(&m_cond, NULL) != 0)
	{
		perror("pthread_cond_init error");
	}
}

CPthreadCond::~CPthreadCond()
{
	if(pthread_cond_destroy(&m_cond) != 0)
	{
		perror("pthread_cond_destroy error");
	}
}

bool CPthreadCond::wait()
{
	if(pthread_cond_wait(&m_cond, &m_mutex) != 0)
	{
		perror("pthread_cond_wait error");
		return false;
	}
	return true;
}

bool CPthreadCond::timewait(unsigned int sec)
{
	struct timeval now;
	struct timespec abstime;
	gettimeofday(&now, NULL);
	abstime.tv_sec = now.tv_sec + sec;
	abstime.tv_nsec = now.tv_usec * 1000;
	this->m_btimeout = false;
	int ret = pthread_cond_timedwait(&m_cond, &m_mutex, &abstime);
	if(ret == ETIMEDOUT)
	{
		m_btimeout = true;
		return false;
	}
	else if(ret != 0)
	{
		perror("pthread_cond_timewait error");
	}
	return ret == 0 ? true : false;
}

bool CPthreadCond::signal()
{
	if(pthread_cond_signal(&m_cond) != 0)
	{
		perror("pthread_cond_signal error");
		return false;
	}
	return true;
}

bool CPthreadCond::broadcast()
{
	if(pthread_cond_broadcast(&m_cond) != 0)
	{
		perror("pthread_cond_broadcast error");
		return false;
	}
	return true;
}

bool CPthreadCond::isTimeout()
{
	return m_btimeout;
}


