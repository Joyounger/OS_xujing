#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
  /* union semun is defined by including <sys/sem.h> */
#else
       /* according to X/OPEN we have to define it ourselves */
  union semun {
        int val;                    /* value for SETVAL */
        struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
        unsigned short int *array;  /* array for GETALL, SETALL */
        struct seminfo *__buf;      /* buffer for IPC_INFO */
  };
#endif



int main()
{
    int semid; 
    pid_t pid,rootpid;
    struct sembuf sop_wait;
    struct sembuf sop_notify;
    union semun arg;
    int i,z,status;
    int n = 5;
    char buf[128],*c;

    if((semid = semget(IPC_PRIVATE,2,0600)) == -1) {
        fprintf(stderr,"%s:semget()\n",strerror(errno));
        exit(1);
    }

    arg.val = 1;
    if(semctl(semid,0,SETVAL,arg) == -1) {
	   perror("semctl(SETALL)");
	   exit(1);
    }

    rootpid=getpid();
	
    sop_wait.sem_num = 0;
    sop_wait.sem_op = -1;
    sop_wait.sem_flg = 0;

    sop_notify.sem_num = 0;
    sop_notify.sem_op = 1;
    sop_notify.sem_flg = 0;

    for(i=1;i<n;++i) {
	    if(pid = fork()) { 
   	        break;
        }
    }
    sprintf(buf,"i:%d cPID:%ld pPID:%ld chPID:%ld\n",i,getpid(),getppid(),pid);
    c = buf;
    z = semop(semid,&sop_wait,1);
    if(z == -1) {
	   perror("semop(sop_wait)");
	   exit(1);
    }
    while(*c !='\0'){
        fputc(*c,stdout);
        c++;
    }
    fputc('\n',stdout);
    z = semop(semid,&sop_notify,1);
    if(z ==-1) {
	   perror("semop(sop_notify)");
    }
    wait(&status);

    if(rootpid==getpid()) {
   	    if(semctl(semid,0,IPC_RMID,arg)==-1) {
            perror("semctl(IPC_RMID)");
            exit(1);
        }
    }

   exit(0);
}
