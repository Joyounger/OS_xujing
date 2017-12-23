#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
 
char * host_name = "127.0.0.1"; // local host
int port = 9889;

int main() 
{
    int cliaddr_len;
    char recvbuf[256],sendbuf[256];
    int sockfd;
    struct sockaddr_in servaddr,cliaddr;
    struct hostent *shost_name;

    if ((shost_name = gethostbyname(host_name)) == 0) {
        perror("Error resolving local host\n");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Error opening socket\n");
        exit(1);
    }

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("call to bind");
    }

    while (1) {
        cliaddr_len = sizeof(cliaddr);
        if (recvfrom(sockfd, recvbuf, 256, 0,(struct sockaddr *)&cliaddr, &cliaddr_len) == -1) {
            perror("Error in receiving response from server\n");
        }
        sprintf(sendbuf, "I got it.\n");
        sendto(sockfd,sendbuf,256,0,(struct sockaddr*)&cliaddr,cliaddr_len);
        printf("\nResponse from server:\n\n%s\n", recvbuf);
        if (strncmp(recvbuf, "stop", 4) == 0) 
            break;
    }

    close(sockfd);
    return 0;
}  
