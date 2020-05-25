#include "CPostThread.h"
#include "CTask.h"
#include "CShm.h"
#include "CSem.h"


extern CShm *shm_1;
extern CSem *sem_wait_shm1;
extern CSem *sem_shm1;

CPostThread::CPostThread(CPthreadPool *pool)
{
	this->pool = pool;
}

CPostThread::~CPostThread()
{

}
	
void CPostThread::Start()
{
    while(1)
    {

        sem_wait_shm1->p();//等待前置服务器写入共享内存
        shm_1->pop(this->buf);//读取共享内存数据
        sem_shm1->v();//共享内存已读出，唤醒下一个写入共享内存

        packet_header_t whead;
        memcpy(&whead, buf, 12);
        
        //根据包头功能号处理相对应任
        //printf("whead.funcId:%d\n", whead.funcId);
        switch (whead.funcId)
        {
            case LOGIN://登陆
            {
                LoginTask *task = new LoginTask(buf);
                this->pool->addtask(task);
            }break;
            case CHANNAL://频道
            {
                ChannelTask *task = new ChannelTask(buf);
                this->pool->addtask(task);
            }break;
            case TYPE://地区
            {
                TypeTask *task = new TypeTask(buf);
                this->pool->addtask(task);
            }break;
            case AREA://类型
            {
                AreaTask *task =new AreaTask(buf);
                this->pool->addtask(task);

            }break;
            case VLIST://视频列表
            {
                VListTask *task = new VListTask(buf);
                this->pool->addtask(task);

            }break;
            case PLAY:
            {
                 PlayTask *task = new PlayTask(buf);
                 this->pool->addtask(task);
            }break;
            case PLAYTIME:
            {
                 PlayTimeTask *task = new PlayTimeTask(buf);
                 this->pool->addtask(task);
            }break;
            default:
            break;
        }
    }
	
}