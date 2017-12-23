#include "shm.h"

int shmid = -1;             /* Shared Memory IPC ID */
int semid = -1;             /* Table locking semaphore */
char *shmp = 0;             /* Pointer to shared memory */
int I = 0;                  /* 0=starter/ 1=second */
int you = 0;                /* 1=second / 0=starter */

struct S_MEM * table;

int main(int argc,char **argv)
{
    union semun semarg;                                         /* for semctl() */
    unsigned short seminit[] = { 1, 0 };                        /* Initial sem values */
    pid_t p1, p2;                                               /* PID for worker 1 & 2 */
    char wbuf[256];                                             /* For fgets() */
    //char rbuf[256];             

    if(argc == 1) {
        atexit(release);
        shmid = shmget(IPC_PRIVATE,sizeof(struct S_MEM),0666);  /* Create Shared Memory */
        if ( shmid == -1 ) {
            perror("shmget()");
            exit(1);
        }
        shmp = shmat(shmid,0,0);                                /* Attach the shared memory */
        if ( shmp == (char *)(-1) ) {
            perror("shmat()");
            exit(1);
        }
        table = (struct S_MEM *)shmp;

        semid = semget(IPC_PRIVATE,2,0666);                     /* Create a binary semaphore set*/
        if ( semid == -1 ) {
            perror("semget()");
            exit(1);
        }
        
        semarg.array = seminit;                                 /* Initialize semaphores  */
        if ( semctl(semid,0,SETALL,semarg) == -1 ) {
            perror("semctl(SETALL)");
            exit(1);
        }
        
        I = 0;
        you = 1;
        /* Initialize shared memory table : */
        LOCK;                                                   /* Wait on semaphore */
        table->semid = semid;                                   /* Make IPC ID public */
        table->worker[0] = getpid();
        table->worker[1] = 0;                                   /* No second yet */
        /* write into shared memory */
        fputs("Write a character string into shared memory: ",stdout);
        fgets(wbuf,sizeof(wbuf),stdin);
        strncpy(table->buf,wbuf,strlen(wbuf)+1);
        UNLOCK;
        printf("\n*** Shared memory IPC ID is # %d ***\n",shmid);
        puts("Waiting for second...");
        WAIT;
        puts("\nTHE communication BEGINS!\n");
        WAIT;
    } else {                                                    /* Second is joining a game : */
        I = 1;                                                  /* We're worker[1] */
        you = 0;                                                /* They're worker[0] */

        shmid = atoi(argv[1]);                                  /* Simple int conversion */

        shmp = shmat(shmid,0,0);                                /* Attach the shared memory */
        if ( shmp == (char *)(-1) ) {
              perror("shmat()");
              exit(1);
        }
        table = (struct S_MEM *)shmp;

        semid = table->semid;                                   /* Locking semaphore ID */

        LOCK;                                                   /* Wait on semaphore */
        p1 = table->worker[0];
        p2 = table->worker[1];
        if ( p2 == 0 )                                          /* No opponent yet? */
            p2 = table->worker[1] = getpid();
        UNLOCK;
        /* Notify semaphore */
        NOTIFY;
        LOCK;
        /* read from shared memory */
        printf("Read the character string from shared memory: %s\n", table->buf);
        UNLOCK;                 
        if (shmdt(shmp) == -1 )
        {
           perror("shmdt()");
           exit(1);
        }
        puts("Operation is over!");
        NOTIFY;
        return 0;
    }
}


