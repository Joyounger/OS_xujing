#include "shm.h"

void release(void) /* Detach shared memory if it is attached */
{ 
    union semun semarg;
    if ( shmp != 0 && shmp != (char *)(-1) )
        if ( shmdt(shmp) == -1 )
            perror("shmdt()");
    
    if ( shmid != -1 )
        if ( shmctl(shmid,IPC_RMID,0) == -1 )  /* Destroy shared memory */
            perror("shmctl(IPC_RMID)");
        
    if ( semid != -1 )
        if ( semctl(semid,0,IPC_RMID,semarg) == -1 ) /* Destroy semaphore */
            perror("semctl(IPC_RMID)");
}
