#ifndef _CSQL_H_
#define _CSQL_H_

#include <iostream>
#include <list>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include "public.h"

using namespace std;


class CSql
{
public:
	CSql();
	~CSql();
	void LoginInit();//登陆数据初始化
	void ChannelInit();//频道数据初始化
	void TypeInit();//类型数据初始化
	void AreaInit();//地区数据初始化
	void AllTypeInit();//所有类型数据初始化

	list<Login_t> LoginList;//登陆数据
	list<VideoChannel_t> ChannelList;//频道数据
	list<VideoType_t> TypeList;//类型数据
	list<VideoArea_t> AreaList;//地区数据
	list<VideoList_t> AllTypeList;//所有类型数据

	

	
};





#endif