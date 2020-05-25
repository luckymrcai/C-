#ifndef __EPOLL_H__
#define __EPOLL_H__

#include <sys/epoll.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include "CSocket.h"
#include "CPthreadPool.h"

class CEpoll
{
public:
	CEpoll(int size);
	int AddEvent(int sockfd);
	int DelEvent(int sockfd);
	int Wait(int maxevents);
	int GetEpoll();
	void Start();
	virtual void Run(int i){};

	int epfd;
	int events_size;
	struct epoll_event *events;
};


class CEpollServer:public CEpoll
{
public:
	CEpollServer(unsigned short port, char *ip, int size, CPthreadPool *pool);
	//~CEpollServer();
	void Run(int i);

	CSocket *server;
	CPthreadPool *pool;
};

#endif
