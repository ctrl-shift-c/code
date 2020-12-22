#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

void jobs(sigset_t sigpend)
{
	while(1)
	{
		for(int i=0; i<31; i++)
		{
			if(sigismember(&sigpend, i))
				printf("1 ");
			else
				printf("0 ");
		}
		printf("\n");
		sleep(3);
	}
}


int main(void)
{
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	sigprocmask(SIG_SETMASK, &set, NULL);
	sigset_t sigpend;
	sigpending(&sigpend);
	jobs(sigpend);
	while(1);
}
