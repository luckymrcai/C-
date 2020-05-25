#include "CTask.h"
#include "CShm.h"
#include "CSem.h"

#include "CPthreadCond.h"
#include "CPthreadTask.h"

CPthreadCond cond_recv_packs;
CPthreadCond cond_logins;
CPthreadCond cond_send_packs;

extern CSql *UserData;
extern CShm *shm_1;
extern CShm *shm_2;
extern CShm *shm_logins;
extern CSem *sem_wait_shm1;
extern CSem *sem_wait_shm2;
extern CSem *sem_shm1;
extern CSem *sem_shm2;

CTask::CTask(char *buf)
{
    memcpy(this->buf, buf, 660);  
    HeadPack(0);
}

//打包包头
void CTask::HeadPack(size_t size)
{
    memcpy(&this->head, this->buf, sizeof(this->head));
    this->head.optid = 0x01;              // 操作码：请求0x00 和 应答0x01
    this->head.usrlenth = size; //包体的长度
    this->head.packet_seq = 10086;        //包序号
    this->head.packet_sum = 1;            //包总数
    this->head.trans_sum = 10086;         //包流水，设备生成从0开始
    strcpy(this->head.srcAddr,"10086");   //预留
    strcpy(this->head.dstAddr,"10086");   //预留
    this->head.syn = 04;                  //判断包头是否正确 0x040x04
}

void CTask::SendPackage(packet_header_t head, char *body)
{
    char w_buf[sizeof(head) + sizeof(body) + sizeof(packet_tail_t) + 1];
    packet_tail_t tail;
    tail.pack_tail = 0x05;
    memcpy(w_buf, &head, sizeof(head));
    memcpy(w_buf + sizeof(head), body, sizeof(body));
    memcpy(w_buf + sizeof(head) + sizeof(body), &tail, sizeof(packet_tail_t));
    
    sem_shm2->p();
    shm_2->push(w_buf,sizeof(w_buf));
    sem_wait_shm2->v();
}

packet_header_t CTask::GetHead()
{
    return this->head;
}

void TestTask::Run()
{
    //接收数据包总数++
    cond_recv_packs.lock();
    CLogThread::recv_packs++;
    log_write(GetHead().funcId,this->buf,GetHead().fd);//写入日志
    cond_recv_packs.unlock();

    sem_shm1->p();//确保一次只能有一个线程写入共享内存，共享内存被读出就可继续写入
    shm_1->push(this->buf, 660);//将包写入共享内存
    sem_wait_shm1->v();//已写入共享内存，唤醒后置服务器读出

    sem_wait_shm2->p();//等待后置服务器写入共享内存
    memset(this->buf, 0, sizeof(buf));
    shm_2->pop(this->buf);//读取共享内存
    sem_shm2->v();//已读出，唤醒共享内存2可继续写入

    memcpy(&this->head, this->buf, HEADSIZE);//填充包头
    write(GetHead().fd, this->buf,sizeof(this->buf));//发送给客户端

    cond_send_packs.lock();
    CLogThread::send_packs++;
    cond_send_packs.unlock();

}

//心跳任务
void HeatTask::Run()
{
    cond_recv_packs.lock();
    CLogThread::recv_packs++;
    cond_recv_packs.unlock();
    Pant_t beat;
    memcpy(&this->head, this->buf, HEADSIZE);
    memcpy(&beat, this->buf + sizeof(HEADSIZE), sizeof(beat));
     
}

//登陆应答包
void LoginTask::Run()
{	
    //打印包体内容
	Login_t login;
	memcpy(&login, this->buf + HEADSIZE, sizeof(Login_t));
	//printf("version:%s username:%s password:%s\n", login.soft_ver, login.username, login.psw);

    //打包包头
	HeadPack(sizeof(Login_t));
    //定义包体
    res_login_t loginres;

    //数据库查询
    list<Login_t>::iterator it;
	for(it = UserData->LoginList.begin();it != UserData->LoginList.end(); it++)
	{
		if(strcmp(login.username, (*it).username) == 0 && strcmp(login.psw, (*it).psw) == 0)
		{	

            //数据库查询操作(后期进行封装)
            sqlite3 *db;
            int ret = sqlite3_open("User.db", &db);
            int row;
            int col;
            char **presult;
            char *err;
            char sql[128]="";
            sprintf(sql,"select id from USerData where Name='%s'",(*it).username);
            sqlite3_get_table(db, sql, &presult, &row, &col, &err);
            sqlite3_close(db);
            //打包包体			
			loginres.user_id = atoi(presult[col]);
			loginres.login_ret = 1;
            //送到共享内存
            SendPackage(GetHead(), (char *)&loginres);

            //printf("发送给head.fd=%d登陆应答包，登陆成功!\n",head.fd);
            
			break;
		}
	}
	if(it == UserData->LoginList.end())
	{
        //登陆应答包
        loginres.user_id = 0;
		loginres.login_ret = 0;
        //送到共享内存
		SendPackage(GetHead(), (char *)&loginres);
        //printf("发送给head.fd=%d登陆应答包，登陆失败!\n",GetHead().fd);
	}

}

//频道应答包
void ChannelTask::Run()
{
    //打包包头
    HeadPack(sizeof(VideoChannel_t));
    //定义包体
    VideoChannel_t channel;

    //数据库查询
    list<VideoChannel_t>::iterator it;
	for(it = UserData->ChannelList.begin(); it != UserData->ChannelList.end(); it++)
	{
        //打包包体    
		channel.classes_count = (*it).classes_count;
		channel.classes_id = (*it).classes_id;
		strcpy(channel.classes_name, (*it).classes_name);

        //送到共享内存
		SendPackage(GetHead(), (char *)&channel);
        //printf("发送给head.fd=%d频道应答包\n",GetHead().fd);
	}
}

//类型应答包
void TypeTask::Run()
{
    //打包包头
    HeadPack(sizeof(VideoType_t));
    //定义包体
    VideoType_t type;

    //数据库查询
    list<VideoType_t>::iterator it;
    for (it = UserData->TypeList.begin(); it != UserData->TypeList.end(); it++)
    {
        //打包包体
        type.type_count = (*it).type_count;
        type.classes_id = (*it).classes_id;
        type.type_id = (*it).type_id;
        strcpy(type.type_name, (*it).type_name);

        //送到共享内存
        SendPackage(GetHead(), (char *)&type);
        //printf("发送给head.fd=%d类型应答包\n",GetHead().fd);
            
    }
}

//地区应答包
void AreaTask::Run()
{  
    //打包包头
    HeadPack(sizeof(VideoArea_t));
    //定义包体
    VideoArea_t area;

    //数据库查询
    list<VideoArea_t>::iterator it;
    for(it = UserData->AreaList.begin(); it != UserData->AreaList.end(); it++)
    {
        //打开包体
        area.area_count = (*it).area_count;
        area.classes_id = (*it).classes_id;
        area.area_id= (*it).area_id;
        strcpy(area.area_name, (*it).area_name);

        //送到共享内存
        SendPackage(GetHead(), (char *)&area);
        //printf("发送给head.fd=%d地区应答包\n",GetHead().fd);
    }
}

//视频信息应答包
void VListTask::Run()
{
    //打包包头
    HeadPack(sizeof(VideoList_t));
    //定义包体
    VideoList_t vlist;

    //数据库查询
    list<VideoList_t>::iterator it;
    for (it = UserData->AllTypeList.begin(); it != UserData->AllTypeList.end(); it++)
    {
        //打包包体
        vlist.video_count = (*it).video_count;
        vlist.video_id = (*it).video_id;
        vlist.classes_id = (*it).classes_id;
        vlist.type_ids = (*it).type_ids;
        vlist.area_id = (*it).area_id;
        strcpy(vlist.video_name, (*it).video_name);              
        vlist.play_times = (*it).play_times;
        vlist.is_new = (*it).is_new;
        strcpy(vlist.video_image, (*it).video_image);
        strcpy(vlist.video_address, (*it).video_address);
        
        //送到共享内存
        SendPackage(GetHead(), (char *)&vlist);
        //printf("发送给head.fd=%d视频信息应答包\n",GetHead().fd);       
    }
}

//视频请求应答包
void PlayTask::Run()
{
	// int uid;
    
 //    list<VideoPlay_t>::iterator it = UserData->AllTypeList.begin();

 //    packet_header_t head;
 //    memcpy(&head, buf, 12);
 //    BDEAL_REQ_T reqbag;
 //    memcpy(&reqbag, buf + 12, 648);

 //    sqlite3 *db;
 //    int ret = sqlite3_open("User.db", &db);
 //    int row; //h
 //    int col; //l
 //    char** presult;
 //    char *err;

 //    sqlite3_get_table(db, "select * from VAddress", &presult, &row, &col, &err);
 //    int i = 1, j = 0;

 //    BDEAL_res_login_t vediores;

 //    strcpy(vediores.adress, presult[i*col]);
 //    vediores.resulte = 1;

 //    for (it = UserData->AllTypeList.begin(); it != UserData->AllTypeList.end(); it++)
 //    {
 //            if (reqbag.video_id == (*it).video_id)
 //            {
 //                    sprintf(vediores.adress, "%s%s", vediores.adress, (*it).video_name);
 //                    break;
 //            }
 //    }
 //    printf("%s\n", vediores.adress);

 //    char wbuf[1024] = "";
 //    memcpy(wbuf, &head, HEADSIZE);
 //    memcpy(wbuf + HEADSIZE, &vediores, sizeof(BDEAL_res_login_t));
 //    write(head.fd, wbuf, HEADSIZE+sizeof(BDEAL_res_login_t));

}

//播放时长
void PlayTimeTask::Run()
{
    
}
