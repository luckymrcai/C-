#ifndef _CPTHREADPOOL_H_
#define _CPTHREADPOOL_H_

#include <stdio.h>
#include <pthread.h>
#include <list>
#include <iostream>
#include "CPthreadCond.h"
#include "public.h"
#include <string.h>
#include "CTask.h"

using namespace std;

class CPthreadPool
{
public:
	CPthreadPool(int minpthread, int maxpthread, int timeout = -1);
	//~CPthreadPool();

	void create_pthread();
	static void *hander(void *arg);
	void addtask(CTask *data);

	int inowpthread;
	int waitpthread;
	int maxpthread;
	int minpthread;

	int timeout;

	CPthreadCond cond;

	list<CTask *>hander_list;
	
};







#endif