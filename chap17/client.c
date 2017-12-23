#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

char * host_name = "127.0.0.1";  /* local host */
int port = 8000;

int main(int argc, char *argv[]) 
{
    char buf[8192];
    char message[256];
    int sockfd, n;
    struct sockaddr_in pin;
    struct hostent *shost_name;

    if ((shost_name = gethostbyname(host_name)) == 0) {
        perror("Error resolving local host\n");
        exit(1);
    }
    
    bzero(&pin, sizeof(pin));
    pin.sin_family = AF_INET;
    pin.sin_addr.s_addr = htonl(INADDR_ANY);
    pin.sin_addr.s_addr = ((struct in_addr *)(shost_name->h_addr))->s_addr;
    pin.sin_port = htons(port);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error opening socket\n");
        exit(1);
    }
    if (connect(sockfd, (void *)&pin, sizeof(pin)) == -1) {
        perror("Error connecting to socket\n");
        exit(1);
    }
    sprintf(message,"What is the time and date\n");
    printf("Sending message %s to server...\n", message);

    if (write(sockfd, message, strlen(message)) == -1) {
        perror("Error in write\n");
        exit(1);
    }
    printf("..sent message.. wait for response...\n");

    while( (n = read(sockfd, buf, 8192)) >0) {
	    buf[n] = 0;
	    if(fputs(buf,stdout) == EOF) {
            perror("Error fputs\n");
	    	exit(1);
	    }
    }
    if(n<0) {
    	printf("\nResponse from server:\n\n%s\n", buf);
	    exit(1);
    }

    close(sockfd);
    return 0;
}
