#ifndef _CTASK_H_
#define _CTASK_H_

#include <sqlite3.h>
#include <list>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "CSql.h"
#include "public.h"


class CTask
{
public:
	CTask(char *buf);

	void HeadPack(size_t size);//打包包头	
	void SendPackage(packet_header_t head, char *body);//发包
	virtual void Run() = 0;
	packet_header_t GetHead();

	char buf[1024];
	packet_header_t head;
};

class TestTask:public CTask
{
public:
	TestTask(char *buf):CTask(buf){};
	~TestTask();
	void Run();	
};

class HeatTask:public CTask
{
public:
	HeatTask(char *buf):CTask(buf){};
	~HeatTask();
	void Run();
};

class LoginTask:public CTask
{
public:
	LoginTask(char *buf):CTask(buf){};
	~LoginTask();
	void Run();
};

class ChannelTask:public CTask
{
public:
	ChannelTask(char *buf):CTask(buf){};
	~ChannelTask();
	void Run();
};

class TypeTask:public CTask
{
public:
	TypeTask(char *buf):CTask(buf){};
	~TypeTask();
	void Run();
};

class AreaTask:public CTask
{
public:
	AreaTask(char *buf):CTask(buf){};
	~AreaTask();
	void Run();
};

class VListTask:public CTask
{
public:
	VListTask(char *buf):CTask(buf){};
	~VListTask();
	void Run();
};

class PlayTask:public CTask
{
public:
	PlayTask(char *buf):CTask(buf){};
	~PlayTask();
	void Run();
};

class PlayTimeTask:public CTask
{
public:
	PlayTimeTask(char *buf):CTask(buf){};
	~PlayTimeTask();
	void Run();
};







#endif