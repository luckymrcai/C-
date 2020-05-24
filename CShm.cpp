#include "CShm.h"


#include <string.h>
#include <stdlib.h>

CShm::CShm(key_t t, size_t size, int shmflg)
{
	this->max = size;
	this->r_now = 0;
	this->w_now = 0;
	this->shmid = shmget((key_t)t, size, 0777| shmflg);
	this->ptr = shmat(shmid, 0, 0);
}

CShm::~CShm()
{
	shmctl(shmid, IPC_RMID, NULL);
}

void CShm::push(char *data, int size)
{
	memset((char *)this->ptr, 0, size);
	memcpy((char *)this->ptr, data, size);
}

void CShm::pop(char *buf)
{
	memcpy(buf, (char *)this->ptr, 660);
}
