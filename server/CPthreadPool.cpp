#include "CPthreadPool.h"

CPthreadPool::CPthreadPool(int minpthread, int maxpthread, int timeout)
{
	this->timeout = timeout;
	this->minpthread = minpthread;
	this->maxpthread = maxpthread;
	this->inowpthread = 0;
	this->waitpthread = 0;

	this->cond.lock();
	for(int i = 0; i < this->minpthread; i++)
	{
		this->create_pthread();
	}
	this->cond.unlock();
}

void CPthreadPool::create_pthread()
{
	this->inowpthread++;
	pthread_t pth = 0;
	pthread_create(&pth, NULL, hander, (void *)this);
	//printf("create %d pthread\n", this->inowpthread);
}

void *CPthreadPool::hander(void *arg)
{
	pthread_detach(pthread_self());
	CPthreadPool *pool = (CPthreadPool *)arg;

	while(1)
	{
		pool->cond.lock();

		if(!pool->hander_list.empty())//Get one task and do it.
		{
			CTask *task = pool->hander_list.front(); 
			pool->hander_list.pop_front();// Remove from task list.

			pool->cond.unlock();
			task->Run();
			delete task;
		}
		else
		{
			pool->waitpthread++;
			if(pool->cond.timewait(pool->timeout))
			{
				pool->waitpthread--;
				pool->cond.unlock();
				if(pool->inowpthread > pool->minpthread)
				{
					//cout << 444 << pool->inowpthread << pool->minpthread << endl;
					pool->inowpthread--;
					break;
				}
			}
			else
			{
				//cout << 200 <<endl;
				pool->waitpthread--;
				pool->cond.unlock();
			}
		}
	}
	pthread_exit(NULL);
}

void CPthreadPool::addtask(CTask *data)
{
	hander_list.push_back(data);
	if(this->waitpthread > 0)
	{
		this->cond.signal();
		//cout << "唤醒\n" << endl;
	}
	else if(this->inowpthread < this->maxpthread)
	{
		this->create_pthread();
		//cout << "创建线程!\n" << this->inowpthread << endl;
	}
}
