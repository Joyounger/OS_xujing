#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *id = "pipe.c is a test unnamed_pipe example\n";
		
int main()
{
    int fd[2];
    char buf[256] = "Hello,everybody:do you understand";

    int n, count = 0;

    write(2,id,strlen(id));
    if(pipe(fd)==-1) {
         perror("pipe");
         exit(1);
    }
    switch(fork()) {
        case 0:      /* child process */
    	    close(fd[0]);
    	    while((count<4096) && ((n = write(fd[1],buf,strlen(buf)))>0)) {
                count +=n;
            }
    	    exit(0);
        case -1:
            perror("fork");
    	    exit(1);
        default:       /* parent process */
            close(fd[1]);
            while((count<4096)&&((n = read(fd[0],buf,strlen(buf)))>0)) {
                count +=n;
            }
            printf("%d bytes of data received from child process:%s\n",count,buf);
    	    exit(0);
    }

    return 0;                                  
}
