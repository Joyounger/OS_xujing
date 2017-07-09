#include <unistd.h>
#include <sys/param.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h> 

int port = 8000;

void init_daemon(void)
{
    pid_t pid;
    int   i;

    if ((pid = fork()) == -1)     /*生成第1个子进程*/
        exit(1);                /* fork失败退出 */

    if (pid > 0)
        exit(0);                /* 父进程退出，使shell成为前台进程*/

    setsid();                   /* 第1子进程成为新会话和新进程组的领头进程的同时也失去控制终端*/

    /* 第1子进程执行下面的操作 */
    for(i=0;i<NOFILE;++i)
        close(i);               /* 关闭已打开的文件描述符 */
    chdir("/rundir");           /*改变当前运行的目录*/
    umask(0);                   /*改变文件创建掩码*/

    return;
}


int main(int argc,char** argv)
{
    int sockfd, tsockfd;
    int addr_size;
    char buf[100];
    struct sockaddr_in s;
    struct sockaddr_in p;
    char temp_buf[256];

    init_daemon();
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1) {
	   perror("socket building failure");
	   exit(1);
    }

    bzero(&s, sizeof(s));
    s.sin_family = AF_INET;
    s.sin_addr.s_addr = INADDR_ANY;
    s.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&s, sizeof(s)) == -1) {
        perror("bind");
        exit(1);
    }

    if(listen(sockfd,10)==-1){
        perror("listen");
        exit(1);
    }

    for(;;) {
   	    tsockfd = accept(sockfd,(struct sockaddr*)&p,&addr_size);
   	    if(tsockfd < 0) {
		    printf("ft:%d\n",errno);
       		perror("accept");
       		exit(1);
   	    }
   	    if(recv(tsockfd,buf,100,0) < 0) {
       		perror("recv");
       		exit(1);
   	    }
        strcpy(temp_buf,"Here is the first file of the president's office.");
   	    if(send(tsockfd,temp_buf,strlen(temp_buf),0) < 0) {
       		perror("send");
       		exit(1);
        }
   	    close(tsockfd);
      	printf("send:%s",temp_buf);
    }
}
