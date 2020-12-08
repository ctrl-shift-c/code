#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>


int main(void)
{
	sigset_t newset;
	sigemptyset(&newset);
	sigaddset(&newset,SIGINT);
	sigprocmask(SIG_SETMASK, &newset, NULL);
	while(1);
}
