#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define LOCK        lockshm(0,1)  /* Lock shared memory */
#define UNLOCK      lockshm(0,0)  /* Unlock shared memory */

#define WAIT       lockshm(1,1)   /* Wait for Notify */
#define NOTIFY     lockshm(1,0)   /* Notify player 1 */

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

struct S_MEM {
    int     semid;                  /* Locking sem IPC ID */
    pid_t   worker[2];              /* Process ID of worker */
    char buf[256];
};

extern struct S_MEM *table;
extern int shmid;                   /* Shared Memory IPC ID */
extern int semid;                   /* Table locking semaphore */
extern char *shmp;                  /* Pointer to shared memory */
extern int I;                       /* 0=starter / 1=challenger */
extern int you;                     /* 1=challenger / 0=starter */

extern void release(void);
extern void lockshm(int sem,int bvar);
