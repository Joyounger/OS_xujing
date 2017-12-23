#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int port = 9889;

int main() {
    int sockfd, n;
    int circle = 0;
    int process;
    char sendbuf[128]; // for sending messages
    char recvbuf[128]; // for receive messages
    struct sockaddr_in addr;
  
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd== -1) {
        perror("Opening socket");
        exit(1);
    }
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(port);
    process = 1;
    do {
      	sprintf(sendbuf,"data packet with ID %d\n", circle);
      	if (circle >10) {
      		sprintf(sendbuf, "stop\n");
    	  	process = 0;
      	}
	    if (sendto(sockfd,sendbuf, sizeof(sendbuf),0,(struct sockaddr *)&addr, sizeof(addr))< 0) {
  	    	perror("Trying to sendto");
      		exit(1);
  	    }
	    n = recvfrom(sockfd,recvbuf, sizeof(recvbuf),0,NULL,NULL);
	    recvbuf[n] = 0;
	    fputs(recvbuf,stdout);
    	circle++;
  } while (process);

  return 0;
}  
