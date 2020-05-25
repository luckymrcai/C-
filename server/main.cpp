#include <iostream>
#include <map>
#include "CEpoll.h"
#include "CSocket.h"
#include "public.h"
#include "CPthreadPool.h"
#include "CShm.h"
#include "CPostThread.h"
#include "CSem.h"
#include "CPthreadTask.h"

using namespace std;

//数据库初始化
CSql *UserData = new CSql;

//共享内存：前置服务器写入，后置服务器读出
CShm *shm_1 = new CShm((key_t)111, 1024, IPC_CREAT);

//共享内存：后置服务器写入，前置服务器读出
CShm *shm_2 = new CShm((key_t)222, 1024, IPC_CREAT);

//共享内存：用户登陆数
CShm *shm_logins = new CShm((key_t)333, 1024, IPC_CREAT);

//信号量：前置服务器等待后置服务器发包过来，准备发包给客户端
CSem *sem_wait_shm1 = new CSem((key_t)444, 1, IPC_CREAT);

//信号量：后置服务器等待前置服务器发包过来，准备解包
CSem *sem_wait_shm2 = new CSem((key_t)555, 1, IPC_CREAT);

//信号量：等待共享内存1被读出
CSem *sem_shm1 = new CSem((key_t)666, 1, IPC_CREAT);

//信号量：等待共享内存2被读出
CSem *sem_shm2 = new CSem((key_t)777, 1, IPC_CREAT);

int main()
{
	sem_wait_shm1->setval(0);//信号量初始值为0
	sem_wait_shm2->setval(0);//信号量初始值为0
	sem_shm1->setval(1);//信号量初始值为1
	sem_shm2->setval(1);//信号量初始值为1
	if(fork() > 0)
	{
		//前置服务器		
		CLogThread log;//运行日志线程
		log.start();

		CPthreadPool pre_pool(20, 50, 5);//创建前置服务器线程池
		CEpollServer epoll(8888, "127.0.0.1", 1024, &pre_pool);
		epoll.Start();
	}
	else
	{
		//后置服务器
		CPthreadPool post_pool(20, 50, 5);//创建后置服务器线程池
		CPostThread post_thread(&post_pool);
		post_thread.Start();

	}
	

	return 0;
}



