#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *id = "rd_pipe.c is a test named_pipe example\n";

int main()
{
	FILE *filp;
	int count = 1;
	char buf[128];
	write(2,id,strlen(id));

	filp = fopen("named_pipe","r");
	if(filp == NULL) {
		perror("fopen");
	    exit(1);
	}
	while((count = fread(buf,1,128,filp))>0) {
	    printf("received from named_pipe:%s\n",buf);
	}
	fclose(filp);

	return 0;
}
