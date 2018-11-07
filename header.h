#define INDSSHMKEY (key_t) 3564
#define OUTDSSHMKEY (key_t) 8761
#define SEMKEY (key_t) 4563
#define SHMSIZE 2000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>

union semun 
{
	int val;
	struct semid_ds *buf;
	unsigned short  *array;
};

typedef struct data
{
	char bline[500];
	int ppid;
	
}data;