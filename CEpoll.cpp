#include "CEpoll.h"
#include "public.h"
#include "CPthreadTask.h"

CEpoll::CEpoll(int size)
{
	this->epfd = epoll_create(size);
	this->events_size = size;
}

int CEpoll::AddEvent(int sockfd)
{
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.events = EPOLLIN;
	ev.data.fd = sockfd;
	return epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);//添加监听事件到epoll中
}

int CEpoll::DelEvent(int sockfd)
{
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.events = EPOLLIN;
	ev.data.fd = sockfd;
	return epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, &ev);//从epoll移出监听事件
}

int CEpoll::Wait(int maxevents)
{
	this->events = new struct epoll_event[maxevents];
	int nfds;
	nfds = epoll_wait(epfd, this->events, maxevents, -1);//阻塞等待事件发生，返回事件的个数
	if(nfds == 0)
	{
		perror("epoll_wait");
	}
	return nfds;
}

int CEpoll::GetEpoll()
{
	return epfd;
}

void CEpoll::Start()
{
	while(1)
	{
		int nevents = this->Wait(this->events_size);
		for(int i = 0;i < nevents;i++)
		{
			this->Run(i);
		}
	}
}

CEpollServer::CEpollServer(unsigned short port, char *ip, int size, CPthreadPool *pool)
:CEpoll(size)
{
	this->server = new CSocket(port, ip);
	server->Listen(20);
	this->AddEvent(this->server->Getfd());
	this->pool = pool;
	
    
}

void CEpollServer::Run(int i)
{
	if(events[i].data.fd == this->server->Getfd())
	{
		if (CLogThread::links < 1010)
		{
			int clientfd = server->Accept();
			this->AddEvent(clientfd);
			CLogThread::links++;
		}
		else
		{
			printf("当前连接数已达上限\n");
		}
		
	}
	else
	{
		char buf[1024] = "";
		int ret = read(events[i].data.fd, buf, 660);
		if(ret <= 0)
		{
			this->DelEvent(events[i].data.fd);
			close(events[i].data.fd);
			if (CLogThread::links > 0)
			{
				CLogThread::links--;
			}	
		}
		else
		{
			packet_header_t head;
			memcpy(&head, buf ,HEADSIZE);
			
			head.fd = events[i].data.fd;
			memcpy(buf, &head, HEADSIZE);


			switch(head.funcId)
			{	
				case BEAT:
                {
                	
                    CLogThread::pants++;//心跳包数量+1
                    HeatTask *task = new HeatTask(buf);
                    this->pool->addtask(task);                    
              	}break;
				case LOGIN:
                {          
                	CLogThread::logins++;      
                    CTask *task = new TestTask(buf);
                    this->pool->addtask(task);

                }break;
                case CHANNAL:
                {
                    CLogThread::channels++;//心跳包数量+1
                    CTask *task = new TestTask(buf);
                    this->pool->addtask(task);
                }break;
                case TYPE:
                {
                    CLogThread::types++;//心跳包数量+1
                    CTask *task = new TestTask(buf);
                    this->pool->addtask(task);
                }break;
                case AREA:
                {
                    CLogThread::areas++;//心跳包数量+1
                    CTask *task = new TestTask(buf);
                    this->pool->addtask(task);
                }break;
                case VLIST:
                {
                    CLogThread::lists++;
                    CTask *task = new TestTask(buf);
                    this->pool->addtask(task);
                }break;
                case PLAY:
                {
                    CLogThread::plays++;
                    CTask *task = new TestTask(buf);
                    this->pool->addtask(task);
                }break;
                case PLAYTIME:
                {
                    CLogThread::play_times++;
                    CTask *task = new TestTask(buf);
                    this->pool->addtask(task);
                }break;
                
			}

		}
	}

}
