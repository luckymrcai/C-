#include "CSem.h"
#include <iostream>
#include <string.h>

using namespace std;

CSem::CSem(key_t key, int nsems, int semflg)
{
	this->semid=semget((key_t)key,nsems,0777 | semflg);

}
CSem::~CSem()
{
	semctl(semid, 0, IPC_RMID);
}

void CSem::p()
{
	struct sembuf sbuf={0,-1,SEM_UNDO};
	semop(semid,&sbuf,1);
}
void CSem::v()
{
	struct sembuf sbuf={0,1,SEM_UNDO};
	semop(semid,&sbuf,1);

}

void CSem::pall()
{
	struct sembuf sbuf={0,-1,SEM_UNDO};
	semop(semid,&sbuf,1);

}
void CSem::vall()
{
	struct sembuf sbuf={0,-1,SEM_UNDO};
	semop(semid,&sbuf,1);
}

void CSem::setval(int val)
{
	union semun semopts;
	semopts.val=val;
	semctl(semid,0,SETVAL,semopts);	
}
int CSem::getvall()
{
	return semctl(semid,0,GETVAL);	
}
void CSem::setall(int val)
{
	
}
void CSem::getall()
{

}

