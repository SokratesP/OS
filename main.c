#include "header.h"
#include "prototypes.h"

int main(int argc, char const *argv[])
{
	int i;
	//user data
	int numberOfProcesses=10;
	int numberOfMessages=10;
	if (argc >= 2){
		numberOfProcesses=atoi(argv[1]);
		numberOfMessages=atoi(argv[2]);
	}
	if (argc < 2)
		printf("Running for 10 processes and 10 lines.\n");
	
	pid_t pid;
	int semaphoreID;
	int inShmID,outShmID;
	data *indsShm,*outdsShm;

	union semun arg;
	union semun gra;
	//values for init of semaphorees
	arg.val=0;
	gra.val=1;

	//cration of semaphores
	semaphoreID=semget(SEMKEY,4,IPC_CREAT|0600);
	semctl(semaphoreID,0,SETVAL,gra);
	semctl(semaphoreID,1,SETVAL,arg);
	semctl(semaphoreID,2,SETVAL,arg);
	semctl(semaphoreID,3,SETVAL,gra);

	//shared memory init
	inShmID=shmget(INDSSHMKEY,SHMSIZE,IPC_CREAT|0600);
	outShmID=shmget(OUTDSSHMKEY,SHMSIZE,IPC_CREAT|0600);
	indsShm=(struct data*)shmat(inShmID,(char*)0,0);
	outdsShm=(struct data*)shmat(outShmID,(char*)0,0);

	for(i = 0; i < numberOfProcesses; i++){
		pid = fork();
		if(pid == 0){
			Producer(numberOfProcesses,semaphoreID,indsShm,outdsShm);
			return 0;
		}else if (pid == -1){
			printf("Something went wrong.Exiting!\n");
			exit(EXIT_FAILURE);
		}
	} 
	/* Parent code C process */
	Consumer(numberOfProcesses,numberOfMessages,semaphoreID,indsShm,outdsShm);

	//deleting semaphores
	semctl(semaphoreID,0,IPC_RMID,0);
	semctl(semaphoreID,1,IPC_RMID,0);
	semctl(semaphoreID,2,IPC_RMID,0);
	semctl(semaphoreID,3,IPC_RMID,0);

	//deleting memory
	shmdt(indsShm);
	shmdt(outdsShm);
	shmctl(inShmID,IPC_RMID,0);	
	shmctl(outShmID,IPC_RMID,0);

	exit(EXIT_SUCCESS);
}