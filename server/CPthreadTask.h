#ifndef _REALTIMELOG_H_
#define _REALTIMELOG_H_


#include <termios.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "CEpoll.h"
#include <pthread.h>



using namespace std;


class CLogThread//实时日志线程
{
public:
	void start();
	static int log_fd;
	static int links;
	static int recv_packs;
	static int send_packs;
	static int logins;
	static int channels;
	static int types;
	static int areas;
	static int lists;
	static int plays;
	static int play_times;
	static int pants;
};

void log_enab(int num);

void log_write(int num,char *buf,int user_id);//日志生成函数

void char_hex(char *buf,char *GetLog);


#endif
