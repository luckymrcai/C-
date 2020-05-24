#ifndef _PUBLIC_H_
#define _PUBLIC_H_

#define BEAT 0000
#define LOGIN 0001
#define CHANNAL 1001
#define TYPE 1002
#define AREA 1003
#define VLIST 1004
#define PLAY 2001
#define PLAYTIME 2002
#define HISTORY 3001



#define HEADSIZE sizeof(packet_header_t)

/************接入层数据包头************/

typedef struct packet_header_st
{
	int fd;//用于前后端通信即目标客户端fd（服务器用到）
	int funcId; // 功能号
		//心跳包-0000，登录包-0001
		//请求视频频道分类-1001，请求视频类型分类-1002，请求视频地区-1003，请求视频列表-1004
		//客户端上传视频点播功能-2001， 客户端上传视频点播时长功能-2002，
		//播放历史请求包-3001

	int optid; // 操作码：请求0x00 和 应答0x01

	int usrlenth;// 包体的长度
	int packet_seq; //包序号
	int packet_sum; //包总数
	int trans_sum;//包流水，设备生成从0开始

	char srcAddr[6]; //预留
	char dstAddr[6]; //预留

	int syn; // 判断包头是否正确 0x040x04
}packet_header_t;

/************接入层数据包尾************/

typedef struct packet_tail_st//包尾,用来验证数据包的完整性
{
	int pack_tail;//设置为0x050x05
}packet_tail_t;

/************数据包报文整体************/

typedef struct  packet_all_st
{
	packet_header_t head;
	char * body;
	packet_tail_st tail;
	//unsigned len; //要发送的数据有效长度
}packet_all_st;

typedef struct pack_back
{
	int syn;
}Pack_Back;

/************业务层数据包体************/

typedef struct Pant//心跳结构体 功能号 0000
{
	int fd; //客户端fd
	int flag;//心跳状态，true-在线，flase-离线
}Pant_t;

//客户端登录请求包
typedef struct login
{
	char soft_ver[6]; //软件版本号
	char username[20]; //用户名
	char psw[20];      //密码
}Login_t;

//登录应答包
typedef struct res_login_st
{
	int user_id; //在线的顺序即用户ID
	int login_ret;	//登录结果： 1-登录成功，0-登录失败
}res_login_t;

//请求视频频道应答包
typedef struct VideoChannel
{
	int classes_count; //频道分类数量
	int classes_id; 	//频道ID
	char classes_name[20];//频道名称
}VideoChannel_t;

//视频类型分类应答包
typedef struct VideoType
{
	int type_count; //视频类型分类数量
	int classes_id;//频道ID
	int type_id;//分类ID	
	char type_name[20];//分类名称
}VideoType_t;

//请求视频地区应答包
typedef struct VideoArea
{
	int area_count; //地区分类数量
	int classes_id;//频道分类ID
	int area_id;//地区分类ID
	char area_name[20];//地区分类名称
}VideoArea_t;

//视频列表应答包
typedef struct VideoList
{
	int video_count; //视频数量
	int video_id;//视频ID
	int classes_id;//频道分类ID
	int type_ids;//类型分类ID,一个视频最多有10个分类
	int area_id;//地区ID
	char video_name[20];//视频名称
	int play_times;//播放次数
	int is_new; //0-旧视频 1-新视频
	char video_image[25]; //视频封面图片链接地址
	char video_address[25]; //视频网络链接地址
}VideoList_t;

/************请求业务交易流水请求包************/
typedef struct TransactionFlow 
{
	int user_id;//用户ID（没有登录的话为0）
	int video_id;//视频ID
	char update_flowid[23]; //上传数据流水；平台生成
}TransactionFlow_t;

//上传视频点播请求包
typedef struct VideoPlay
{
	int user_id;//用户ID（没有登录的话为0）
	int video_id;//视频ID
	char update_flowid[23]; //上传数据流水；平台生成
}VideoPlay_t;

//上传视频点播时长请求包（没有登录的话不需要请求）
typedef struct VideoRecord
{
	int user_id;//用户ID
	int video_id;//视频ID
	int video_seek;//播放时长
	char update_flowid[23]; //上传数据流水；平台生成
}VideoRecord_t;

//上传应答包
typedef struct UpLoad
{
	int results;//上传结果，0-上传成功，1-上传失败
	char update_flowid[23]; //上传数据流水；平台生成
}UpLoad_t;


//播放历史应答包
typedef struct PlayHistory
{
	int video_id;//视频ID
	char video_name[18];//视频名称
	int play_times;//播放次数
	int video_seek;//播放时长
}PlayHistory_t;

typedef struct image
{
	char img_name[20];
	char image[257];
}Image_t;

typedef struct Realtimelog
{
	int Eff_conn_num;//有效连接数
	int Rec_pack_num;//接包数
	int Con_pack_num;//发包数
	int User_login_service_num;//用户登录业务数
	int Gets_videolist_num;//获取视频列表数
	int Gets_videorecord_num;//获取视频播放记录数
	int UpLoad_videorecord_num;//上传视频播放记录数
	int UpLoad_videoTime_num;//上传视频播放时间记录数
}Realtimelog_t;




#endif /*_PUBLIC_H_*/

