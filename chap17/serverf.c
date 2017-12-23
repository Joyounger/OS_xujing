#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>      
#include <time.h>

int port = 8000;

int main() 
{
    struct sockaddr_in serveraddr;
    struct sockaddr_in clientaddr;
    int sockfd,temp_sockfd;
    int clientaddr_size;
    char buf[16384];
    time_t ticks;
    pid_t pid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("call to socket");
        exit(1);
    }
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1) {
        perror("call to bind");
        exit(1);
    }
    if (listen(sockfd, 20) == -1) {
        perror("call to listen");
        exit(1);
    }
    printf("Accepting connections ...\n"); 
  
    while(1) {
        temp_sockfd =accept(sockfd, (struct sockaddr *)&clientaddr,&clientaddr_size);
        if ((pid = fork())== 0) {  /* child process the request */
            close(sockfd);    
            if (read(temp_sockfd, buf, 16384) == -1) {
                perror("call to read");
                exit(1);
            }
            printf("received from client:%s\n", buf);
            ticks = time(NULL);
            sprintf(buf,"%s\r\n",ctime(&ticks));

            if (write(temp_sockfd, buf,strlen(buf)) == -1) {
                perror("call to write");
                exit(1);
            }
            close(temp_sockfd);
            exit(0);
        }
        close(temp_sockfd);
    }

    return 0;
}
