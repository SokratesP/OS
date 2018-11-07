#include "header.h"

//semaphore operation values
struct sembuf CWU={0,1,0};

struct sembuf CRU={1,1,0};

struct sembuf FRD={2,-1,0};

struct sembuf FWD={3,-1,0};


int Consumer(int numberOfProcesses,int numberOfMessages,int semaphoreID,data *indsShm,data *outdsShm){
	int i;
	int y;
	int SpidMatch=0;
	int child_status;
	data tempData;

	printf("Consumer with pid=%d was born\n",getpid());
	for (y = 0; y < numberOfMessages; y++){	
		//-1 = no other consumer can read from inds
		semop(semaphoreID,&FRD,1);

		//getting data from inds
		strcpy(tempData.bline,indsShm->bline);
		tempData.ppid=indsShm->ppid;
		
		//+1 = producers can write to inds now that c has got its data
		semop(semaphoreID,&CWU,1);
		
		//capitalizing
		i=0;
		while (tempData.bline[i] != EOF) 
		{           												
    		if (tempData.bline[i] >= 'a' && tempData.bline[i] <= 'z')       
      			tempData.bline[i] = tempData.bline[i] - ('a'-'A'); 				
    		i++;                              						
  		}
  		
  		//-1 = no other consumer can write to outds
  		semop(semaphoreID,&FWD,1);
		
		//transfering data
		outdsShm->ppid=tempData.ppid;
		strcpy(outdsShm->bline,tempData.bline);
		
		//+1 = other producers can read from outds
		semop(semaphoreID,&CRU,1);
	}
	
	//closing producers
	for(y = 0; y < numberOfProcesses; y++){
		//-1 = no other consumer can write to outds
		semop(semaphoreID,&FWD,1);
		
		outdsShm->ppid=getpid();
		strcpy(outdsShm->bline,"Closing");
		
		//+1 = producers can write to inds now that c has got its data
		semop(semaphoreID,&CWU,1);
		
		//+1 = other producers can read from outds
		semop(semaphoreID,&CRU,1);
		
		//geting producer status (pidmatch)
		wait(&child_status);
		SpidMatch += WEXITSTATUS(child_status);
	}	
	printf("Closing Consumer with the following stats :\n-> Number of process = %d\n-> Number of messages = %d\n-> Sum of pidmatch = %d\n",numberOfProcesses,numberOfMessages,SpidMatch);
	return 0;
}