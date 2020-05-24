#include "CPthreadTask.h"
#include "public.h"
#include <string.h>
#include <list>


using namespace std;
list<Pant_t> *pant;//心跳链表

/*实时日志的互斥锁*/
pthread_mutex_t log_mutex;
pthread_mutex_t channel_mutex;
pthread_mutex_t type_mutex;
pthread_mutex_t msg_mutex;
pthread_mutex_t play_mutex;
pthread_mutex_t record_mutex;
pthread_mutex_t send_mutex;
pthread_mutex_t pant_mutex;


int CLogThread::log_fd = 0;
int CLogThread::links = 0;
int CLogThread::recv_packs = 0;
int CLogThread::send_packs = 0;
int CLogThread::logins = 0;
int CLogThread::channels = 0;
int CLogThread::types = 0;
int CLogThread::areas = 0;
int CLogThread::lists = 0;
int CLogThread::plays = 0;
int CLogThread::play_times = 0;
int CLogThread::pants = 0;



/*******************************************实时日志显示线程*******************************************************************************************************/

void * LogTask(void *arg)
{
	while (1)
	{
		sleep(1);
		printf("\\***********************************************************\\\n");
		printf("有效连接数：%d\n",CLogThread::links);
		printf("接收数据包：%d  发送数据包：%d  心跳包：%d\n\n",CLogThread::recv_packs,CLogThread::send_packs,CLogThread::pants);
		printf("接收登陆包：%d\n",CLogThread::logins);
		printf("获取视频频道：%d  获取视频分类：%d  获取视频地区：%d\n",CLogThread::channels,CLogThread::types,CLogThread::areas);
		printf("获取视频列表：%d  上传播放数据：%d  上传播放时长：%d\n",CLogThread::lists,CLogThread::plays,CLogThread::play_times);
		printf("\\***********************************************************\\\n");
	}
}

void CLogThread::start()
{
	//创建当天日志
	char str[50] = {0};
	time_t timep;
	struct tm *t;
	time(&timep);
	t = localtime(&timep);
	sprintf(str,"Log/%d-%d-%d.log",(1900+t->tm_year),(1+t->tm_mon),t->tm_mday);
	CLogThread::log_fd = open(str,O_RDWR | O_CREAT | O_APPEND);

	pthread_t m_tid;
	if(pthread_create(&m_tid,NULL,LogTask,(void *)this) != 0)
    {
        perror("create thread error : ");
    }
}

/*******************************************日志生成函数*******************************************************************************************************/
void log_enab(int num)
{
	// if (num == 1000 && sem->GetVal(2) == 1)
	// {
	// 	sem->SetVal(2,0);
	// }
	// else if (num != 1000 && sem->GetVal(2) == 0)
	// {
	// 	sem->SetVal(2,1);
	// }
}

void log_write(int num,char *buf,int user_id)
{
	char log[2000] = {0};
	char str[100] = {0};
	time_t timep;
	struct tm *t;
	time(&timep);
	t = localtime(&timep);
	sprintf(str,"\r\n时间：%d-%d-%d %d:%d:%d\r\n",(1900+t->tm_year),(1+t->tm_mon),t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
	strcat(log,str);
	switch (num)
	{
	case 1001:
		strcat(log,"功能：心跳处理\r\n");
		break;
	case 1002:
		strcat(log,"功能：登录\r\n");
		break;
	case 2001:
		strcat(log,"功能：视频频道获取\r\n");
		break;
	case 2002:
		strcat(log,"功能：视频分类获取\r\n");
		break;
	case 2003:
		strcat(log,"功能：视频地区获取\r\n");
		break;
	case 2004:
		strcat(log,"功能：视频列表获取\r\n");
		break;
	case 3001:
		strcat(log,"功能：上传播放数据\r\n");
		break;
	case 3002:
		strcat(log,"功能：上传播放时长\r\n");
		break;
	case 4001:
		strcat(log,"功能：播放历史获取\r\n");
		break;
	}
	strcat(log,"类型：接收\r\n");
	if (num == 1001)
	{
		sprintf(str,"客户端FD：%d\r\n",user_id);
	}
	else
		sprintf(str,"用户ID：%d\r\n",user_id);
	strcat(log,str);
	strcat(log,"数据包：\r\n");
	char_hex(buf,log);
	flock(CLogThread::log_fd,LOCK_EX);
	write(CLogThread::log_fd,log,strlen(log));
	fsync(CLogThread::log_fd);//刷新fd内容到磁盘
	flock(CLogThread::log_fd,LOCK_UN);
}


void char_hex(char *buf,char *GetLog)
{
	int i,j;
	int len = 64;
	char acBuf[64] = {0};
	unsigned char tmp[512];
	memset(tmp,0,512);
	memcpy(tmp,buf,len);  
	memset(acBuf, 0x00 ,64);
	for(i=0;i<len/16+1;i++)
	{
		if((i*16) >= len)
		{
			break;
		}
		for(j=0;j<16;j++)
		{
			if((i*16+j) >= len)
			{
				break;
			}
			sprintf(acBuf,"%02x",tmp[i*16+j]);
			strcat(GetLog, acBuf);
			memset(acBuf, 0x00 ,64);

			if(((j+1)%4 != 0 ) && ((i*16+j)<(len-1)))
			{
				strcat(GetLog, " ");
			}
			if((j+1)%4 == 0 )
			{
				strcat(GetLog, " ");
			}
		}
		if(len/16 == i)
		{
			for(j=0;j<((16-(len%16)-1))*3;j++)
			{
				strcat(GetLog, " ");
			}
		}
		strcat(GetLog, "\r\n");
	}
}
