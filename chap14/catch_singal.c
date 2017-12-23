#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

char *id = "SIGINT come from key interruption: intr = ^C\n";
int count=0;

static void catch_signal(int sig)
{
    ++count;
    write(1,id,strlen(id));
}

int main(int argc,char *argv[])
{
    struct sigaction sa_oact;
    struct sigaction sa_nact;

    sa_nact.sa_handler = catch_signal;
    sa_nact.sa_flags = 0;
    sigemptyset(&sa_nact.sa_mask);
    sigaction(SIGINT,&sa_nact,&sa_oact);
    puts("Can you give me a signal? ");
    puts("Please press ctrl+c!");

    do {
	sleep(2);
    } while(count < 1);
    puts("ended");

    return 0;
}
