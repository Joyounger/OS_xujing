#include "shm.h"

void lockshm(int sem,int bvar)      /* Peform semaphore wait/notifies */
{ 
    int z;                          /* Return status */
    static struct sembuf sops = { 0, -1, 0 };
   
    sops.sem_num = sem;             /* Select semaphore */
    sops.sem_op = bvar ? -1 : 1;    /* Wait / Notify */
    do  {
        z = semop(semid,&sops,1);   /* Semaphore operation */
    } while ( z == -1 && errno == EINTR );

    if ( z == -1 ) {
        perror("semop()");         
        exit(13);
    }
}
