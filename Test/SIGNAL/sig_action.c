#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

void SigAction(int n)
{
	printf("%d\n", n);
}

void jobs(sigset_t sigpend)
{
		for(int i=1; i<32; i++)
		{
			if(sigismember(&sigpend, i))
				printf("1 ");
			else
				printf("0 ");
		}
		printf("\n");
		sleep(1);
}

int main(void)
{
	//nsigac.sa_handler = SIG_IGN;
	//nsigac.sa_handler = SIG_DFL;
	/*sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	sigprocmask(SIG_SETMASK, &set, NULL);*/
	pid_t pid;
	pid = fork();
	if(pid == 0)
	{
		sigset_t sigpend;
		sigpending(&sigpend);
		/*
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
			usleep(100);
		}
		*/
		//while(1);
	}
	else if(pid > 0)
	{
		struct sigaction nsigac, osigac;
		sigset_t s,se;
		/*
		sigemptyset(&s);
		sigaddset(&s, SIGINT);
		sigprocmask(SIG_SETMASK, &s, NULL);
		printf("child create ..\n");
		*/
		nsigac.sa_handler = SIG_IGN;
		nsigac.sa_handler = SigAction;
		nsigac.sa_flags = 0;
		sigemptyset(&nsigac.sa_mask);
		sigaction(SIGINT, &nsigac, &osigac);
		while(1)
		{
			sigpending(&se);
			jobs(se);
			//pause();
		}
	}
	else
	{
		printf("fork error..\n");
	}
	return 0;
}
