#ifndef _CSHM_H_
#define _CSHM_H_

#include <sys/ipc.h>
#include <sys/shm.h>

class CShm
{
public:
	CShm(key_t t, size_t size, int shmflg);
	~CShm();
	void push(char *data, int size);
	void pop(char *buf);

	int shmid;
	int max;
	int w_now;
	int r_now;
	void *ptr;
	
};














#endif