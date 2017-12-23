#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>

struct msgbuf {
    long mtype;
    char mtext[80];
} msq_buf;


int main() 
{
    key_t key;
    int msgid;
    int i;

    // Open a message queue:
    msgid = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    printf("message queue msgid=%d\n", msgid);
    if (msgid == -1) {
        perror("msgget(access)");
        exit(1);
    }

    // Send a message:
    printf("Sending a print message...\n");
    msq_buf.mtype = 1;
    sprintf(msq_buf.mtext,"print a message");
    if (msgsnd(msgid, (struct msgbuf *)&msq_buf, sizeof("print a message") + 1, 0) == -1) { 
        perror("msgsend");
        printf("Error sending message\n");
        exit(1);
    }
    
    // Receive message:
    msq_buf.mtext[0] = 0;  
    i = msgrcv(msgid, (struct msgbuf *)&msq_buf, 80, 1, IPC_NOWAIT);
    if (i == -1) { 
        printf("no message is avaliable of type 1\n");
    } else {
        printf("message of type 1 received with data: %s\n", msq_buf.mtext);
    }

    // Now remove the message queue:
    msgctl(msgid, IPC_RMID, 0);

    return 0;
}
