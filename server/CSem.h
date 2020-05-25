#ifndef _CSEM_H_
#define _CSEM_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun
{
     int val;
     struct semid_ds *buf;
     unsigned short int *array;
     struct seminfo *__buf;
};
class CSem
{
public:

	CSem(key_t key, int nsems, int semflg);
	~CSem();

	void p();
	void v();
	void pall();
	void vall();
	void setval(int val);
	int  getvall();
	void setall(int val);
	void getall();


	int semid;


	union semun semopts;

};






#endif /*_SEM_H_*/