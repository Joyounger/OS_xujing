#include <unistd.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/types.h>
#include <signal.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_daemon(void)
{
    pid_t pid;
    int   i;
  
    if ((pid = fork())==-1)     /*生成第1个子进程*/
   	    exit(1);                /* fork失败退出 */
	   
    if (pid > 0)
	    exit(0);                /* 父进程退出，使shell成为前台进程*/ 

    setsid();                   /* 第1子进程成为新会话和新进程组的领头进程的同时也失去控制终端*/
   
    if ((pid = fork())==-1)     /* 由第1个子进程生成子进程 */
        exit(1);                /* 第1个子进程fork失败，程序退出*/
    if(pid>0)
        exit(0);                /* 第1个子进程退出*/

   /* 第2子进程执行下面的操作 */
    for(i=0;i<NOFILE;++i)
        close(i);               /* 关闭已打开的文件描述符 */
    chdir("/rundir");           /*改变当前运行的目录*/
    umask(0);                   /*改变文件创建掩码*/

    return;
}


void init_daemon(void);

int main()
{
    FILE *fp;
    FILE *p;
    char buf[256];

    init_daemon(); 

    while(1) {
        sleep(30);
        if((fp=fopen("test.log", "a"))>=0) {
            p=popen("ps -f","r");
            while(fgets(buf,sizeof(buf),p)!=0)
                fprintf(fp,"%s\n",buf);
                pclose(p);
        }
        fclose(fp);
    }

    return 0;
}








