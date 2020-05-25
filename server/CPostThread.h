#ifndef _CPOSTPTHREAD_H_
#define _CPOSTPTHREAD_H_

#include "CPthreadPool.h"

class CPostThread
{
public:
	CPostThread(CPthreadPool *pool);
	~CPostThread();
	
	void Start();	
	CPthreadPool *pool;
	char buf[1024];
};









#endif