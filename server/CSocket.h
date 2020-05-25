#ifndef _CSOCKET_H_
#define _CSOCKET_H_

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

class CSocket
{
public:
	CSocket(unsigned short port, char *ip, int type = SOCK_STREAM);

	~CSocket();

	int Bind();
	int Listen(int backlog);
	int Accept();
	int Read(char *buf, int len);
	int Write(char *buf, int len);
	int Getfd();;

private:
	int sockfd;
	char m_ip[20];
	unsigned short m_port;
	
};






#endif