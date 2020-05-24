#include "CSql.h"

CSql::CSql()
{
	this->LoginInit();
	this->ChannelInit();
	this->TypeInit();
	this->AreaInit();
	this->AllTypeInit();
}

CSql::~CSql()
{

}

//登陆数据初始化
void CSql::LoginInit()
{
	Login_t login;
	//list<Login_t>::iterator it;
	sqlite3 *db;
	int ret = sqlite3_open("User.db", &db);
	int row;
	int col;
	char **presult;
	char *err;

	sqlite3_get_table(db, "select * from USerData", &presult, &row, &col, &err);
	int i = 0;
	for(i = 0; i < row; i++)
	{
		if(i == 0)
		{
			cout << "———————————————————————————————————————————————————"<<endl;
			cout << presult[0] << "\t"<< presult[1] <<"\t" << presult[2] <<"\t" << presult[3] << endl;
		}
		else
		{
			memset(&login, 0, sizeof(login));
			cout << presult[i*col] << "\t" << presult[i*col + 1] <<"\t" << presult[i*col + 2] <<"\t" << presult[i*col + 3] << endl;
			//login.id = atoi(presult[i*col]);
			strcpy(login.username, presult[i*col + 1]);
			strcpy(login.psw, presult[i*col + 2]);
			strcpy(login.soft_ver, presult[i*col + 3]);
			this->LoginList.push_back(login);
		}
		
	}
	cout << "———————————————————————————————————————————————————"<<endl;

	// for(it = this->LoginList.begin(); it != this->LoginList.end(); it++)
	// {
	// 	cout << (*it).id << (*it).username << (*it).psw << (*it).soft_ver << endl;
	// }
	sqlite3_close(db);
	
}

//频道数据初始化
void CSql::ChannelInit()
{
	VideoChannel_t channeltype;;
	sqlite3 *db;
	int ret = sqlite3_open("User.db", &db);
	int row;
	int col;
	char **presult;
	char *err;

	sqlite3_get_table(db, "select * from VideoChannel", &presult, &row, &col, &err);
	int i = 0;
	for(i = 0; i < row; i++)
	{
		if(i == 0)
		{
			cout << "———————————————————————————————————————————————————"<<endl;
			cout <<presult[0] << "\t"<< presult[1] <<"\t" << presult[2] << endl;
		}
		else
		{
			memset(&channeltype, 0, sizeof(channeltype));
			cout << presult[i*col] << "\t" << presult[i*col + 1] <<"\t" << presult[i*col + 2] << endl;
			channeltype.classes_count = atoi(presult[i*col]);
			channeltype.classes_id = atoi(presult[i*col + 1] );
			strcpy(channeltype.classes_name, presult[i*col + 2]);
			this->ChannelList.push_back(channeltype);
		}
		
	}
	cout << "———————————————————————————————————————————————————"<<endl;

	// for(it = this->LoginList.begin(); it != this->LoginList.end(); it++)
	// {
	// 	cout << (*it).id << (*it).username << (*it).psw << (*it).soft_ver << endl;
	// }
	sqlite3_close(db);

}

//类型数据初始化
void CSql::TypeInit()
{
	VideoType_t video_type;;
	sqlite3 *db;
	int ret = sqlite3_open("User.db", &db);
	int row;
	int col;
	char **presult;
	char *err;

	sqlite3_get_table(db, "select * from VideoType_t", &presult, &row, &col, &err);
	int i = 0;
	for(i = 0; i < row; i++)
	{
		if(i == 0)
		{
			cout << "———————————————————————————————————————————————————"<<endl;
			cout <<presult[0] << "\t"<< presult[1] <<"\t" << presult[2] << "\t" << presult[3] << endl;
		}
		else
		{
			memset(&video_type, 0, sizeof(video_type));
			cout << presult[i*col] << "\t" << presult[i*col + 1] <<"\t" << presult[i*col + 2] << "\t" << presult[i*col + 2] << endl;
			video_type.type_count = atoi(presult[i*col]);
			video_type.classes_id = atoi(presult[i*col+ 1] );
			video_type.type_id = atoi(presult[i*col+ 2] );
			strcpy(video_type.type_name, presult[i*col + 3]);
			this->TypeList.push_back(video_type);
		}
		
	}
	cout << "———————————————————————————————————————————————————"<<endl;

	// for(it = this->LoginList.begin(); it != this->LoginList.end(); it++)
	// {
	// 	cout << (*it).id << (*it).username << (*it).psw << (*it).soft_ver << endl;
	// }
	sqlite3_close(db);

}

//地区数据初始化
void CSql::AreaInit()
{
	VideoArea_t AreaType;;
	sqlite3 *db;
	int ret = sqlite3_open("User.db", &db);
	int row;
	int col;
	char **presult;
	char *err;

	sqlite3_get_table(db, "select * from VideoType_t", &presult, &row, &col, &err);
	int i = 0;
	for(i = 0; i < row; i++)
	{
		if(i == 0)
		{
			cout << "———————————————————————————————————————————————————"<<endl;
			cout <<presult[0] << "\t"<< presult[1] <<"\t" << presult[2] << "\t" << presult[3] << endl;
		}
		else
		{
			memset(&AreaType, 0, sizeof(AreaType));
			cout << presult[i*col] << "\t" << presult[i*col + 1] <<"\t" << presult[i*col + 2] << "\t" << presult[i*col + 3] << endl;
			AreaType.area_count = atoi(presult[i*col]);
			AreaType.classes_id = atoi(presult[i*col+ 1] );
			AreaType.area_id = atoi(presult[i*col+ 2] );
			strcpy(AreaType.area_name, presult[i*col + 3]);

			this->AreaList.push_back(AreaType);
		}
		
	}
	cout << "———————————————————————————————————————————————————"<<endl;

	// for(it = this->LoginList.begin(); it != this->LoginList.end(); it++)
	// {
	// 	cout << (*it).id << (*it).username << (*it).psw << (*it).soft_ver << endl;
	// }
	sqlite3_close(db);
}

//所有类型数据初始化
void CSql::AllTypeInit()
{
	VideoList alltype;;
	sqlite3 *db;
	int ret = sqlite3_open("User.db", &db);
	int row;
	int col;
	char **presult;
	char *err;

	sqlite3_get_table(db, "select * from VideoAllType", &presult, &row, &col, &err);
	int i = 0;
	for(i = 0; i < row; i++)
	{
		if(i == 0)
		{
			cout << "———————————————————————————————————————————————————"<<endl;
			cout << presult[0] << "\t" << presult[1] << "\t" << presult[2] << "\t" << presult[3] << "\t"
				 << presult[4] << "\t" << presult[5] << "\t" << presult[6] << "\t" << presult[7] << "\t"
			 	 << presult[8] << "\t" << presult[9] << endl;
		}
		else
		{
			memset(&alltype, 0, sizeof(alltype));
			cout << presult[i*col] << "\t" << presult[i*col + 1] <<"\t" << presult[i*col + 2] << "\t"
				 << presult[i*col + 3] << "\t" << presult[i*col + 4] <<"\t" << presult[i*col + 5] << "\t"
				 << presult[i*col + 6] << "\t" << presult[i*col + 7] <<"\t" << presult[i*col + 8] << "\t"
				 << presult[i*col + 9] << endl;

			alltype.video_count = atoi(presult[i*col]);
			alltype.video_id = atoi(presult[i*col+ 1] );
			alltype.classes_id = atoi(presult[i*col+ 2] );			
			alltype.area_id = atoi(presult[i*col+ 3] );
			alltype.type_ids = atoi(presult[i*col+ 4] );
			strcpy(alltype.video_name, presult[i*col + 5]);
			alltype.play_times = atoi(presult[i*col+ 6] );
			alltype.is_new = atoi(presult[i*col+ 7] );
			strcpy(alltype.video_image, presult[i*col + 8]);
			strcpy(alltype.video_address, presult[i*col + 9]);

			this->AllTypeList.push_back(alltype);
		}
		
	}
	cout << "———————————————————————————————————————————————————"<<endl;

	
	sqlite3_close(db);
}
