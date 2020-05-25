#include "CSocket.h"
#include "public.h"
CSocket::CSocket(unsigned short port, char *ip, int type)
{
	int opt = 1;
	strcpy(this->m_ip, ip);
	this->m_port = port;
	this->sockfd = socket(AF_INET, type, 0);
	if(this->sockfd == -1)
	{
		perror("socket");
		exit(1);
	}
	printf("socket ok!\n");
	setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));//异常关闭，自动解绑端口	
	Bind();

}

int CSocket::Bind()
{
	struct sockaddr_in servaddr;
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(this->m_port);

	if(bind(this->sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
	{
		perror("bind");
		exit(1);
	}
	printf("bind ok!\n");
	return this->sockfd;
}

int CSocket::Listen(int backlog)
{
	if(listen(this->sockfd, backlog) == -1)
	{
		perror("listen");
		exit(1);
	}
	printf("listen ok!\n");
	return this->sockfd;

}

int CSocket::Accept()
{
	struct sockaddr_in cliaddr;
	socklen_t cliaddrlen;
	cliaddrlen = sizeof(cliaddr);
	
	int clifd = accept(this->sockfd, (struct sockaddr *)&cliaddr, &cliaddrlen);
	if(clifd == -1)
	{
		perror("accept");
	}
	else
	{
		//printf("accept ok\n");
		//printf("accept a new client:%s:%d\n",inet_ntoa(cliaddr.sin_addr),cliaddr.sin_port);
		// Pant_t *t;
		// printf("wait client\n");
		// read(clifd, t,sizeof(t));
		// printf("fd = %d, flag = %d\n", t->fd,t->flag);
	}
	return clifd;
	
}

int CSocket::Read(char *buf, int len)
{
	return read(this->sockfd, buf, len);
}

int CSocket::Write(char *buf, int len)
{
	return write(this->sockfd, buf, len);
}

int CSocket::Getfd()
{
	return this->sockfd;
}





