#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc,char **argv)
{
	int sockfd,numb;
	struct sockaddr_in s;
	struct hostent *host;
	char buf[100];
	int port = 8000;

	if(argc != 2) {
		fprintf(stderr,"usage:%s hostname\n",argv[0]);
	   	exit(1);
	}

	if(!(host = gethostbyname(argv[1]))) {
		perror("error in resolving hostname");
		exit(1);
	}

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0) {
	   	perror("socket");
	   	exit(1);
	}

	bzero(&s,sizeof(s));
	s.sin_family=AF_INET;
	s.sin_addr.s_addr=((struct in_addr *)(host->h_addr))->s_addr;
	s.sin_port=htons(port);

	if((connect(sockfd,(struct sockaddr*)&s,sizeof(s))) != 0) {
	   perror("connect");
	   exit(1);
	}

	printf("get connected!\n");
	getchar();
	strcpy(buf,"Give me the first file of the president's office");
	if(send(sockfd,buf,strlen(buf),0) < 0) {
	   perror("send");
	   exit(1);
	}

	numb = recv(sockfd,buf,100,0);
	if(numb<0) {
	   perror("recv");
	   exit(1);
	}
	buf[numb]='\0';
	printf("Received=%s\n",buf);
	close(sockfd);

	return 0;
}
