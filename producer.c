#include "header.h"

//semaphor operation values
struct sembuf CWD={0,-1,0};

struct sembuf CRD={1,-1,0};

struct sembuf FRU={2,1,0};

struct sembuf FWU={3,1,0};

int Producer(int numberOfProcesses,int semaphorID,data *indsShm,data *outdsShm){
	int i;
	int pidmatch=0;
	int flag=1;
	FILE *randomFile;
	data tempData;

	printf("Producer with pid=%d was born.\n",getpid());

	while(flag){
		//-1 = no other producer can write in inds
		semop(semaphorID,&CWD,1);
		
		//getting line and temporary storing data
		randomFile = fopen("randomfile.txt", "r");
		for (i = 0; i < 119; i++)
			tempData.bline[i] = getc(randomFile);
		tempData.bline[i]='\0';
		tempData.ppid=getpid();

		//passing data to inds
		indsShm->ppid=tempData.ppid; 
		strcpy(indsShm->bline,tempData.bline);
		
		//+1 = consumer can read from inds now that something is on it
		semop(semaphorID,&FRU,1);
		
		//-1 = no other producer can read from outds. this producer is accesing it
		semop(semaphorID,&CRD,1);

		//getting data from outds
		strcpy(tempData.bline,outdsShm->bline);
		tempData.ppid=outdsShm->ppid;
		
		//ending message
		if(strcmp(tempData.bline,"Closing")==0){
			flag=0;
			printf("Closing Producer with pid : %d\n",getpid());
		}else{
			if(tempData.ppid == getpid())
				printf("%s processed by pid = %d same as the current one!\n",tempData.bline,tempData.ppid);
			else
				printf("%s processed by pid = %d current process pid is %d\n",tempData.bline,tempData.ppid,getpid());
		}
		
		//checking for pid match
		if (tempData.ppid==getpid())
			pidmatch++;

		//+1 = consumer can write in outds. producer has "cleared" it.
		semop(semaphorID,&FWU,1);
	}
	exit(pidmatch);
}

