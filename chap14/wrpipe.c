#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *id = "wr_pipe.c is a test named_pipe example\n";
int main()
{
	FILE *filp;
	int count = 1;
	char buf[128];
	write(2,id,strlen(id));

	filp = fopen("named_pipe","w");
	if(filp ==NULL){
		perror("fopen");
	    exit(1);
	}
	sprintf(buf,"It is really interesting,I like this experience.\n");
	fwrite(buf,1,strlen(buf),filp);
	fclose(filp);

	return 0;
}
