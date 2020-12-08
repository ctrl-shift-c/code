#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
	pid_t pid;
	pid = fork();
	if(pid > 0)
	{
		printf("parent waiting..\n");
		//pid_t wpid = wait(NULL);
		pid_t wpid;
		while((wpid  = waitpid(-1, NULL, WNOHANG))!= -1)
		{
			if(wpid > 0)
			{
				printf("parent wait child success zobie process pid [%d]\n",wpid);
			}
			if(wpid == 0)
			{
				printf("parent runing.....\n");
				usleep(100000);
			}
		}
		printf("parent wait child success wpid %d\n", wpid);
		while(1);
	}
	else if(pid == 0)
	{
		int flags = 10;
		while(flags--)
		{
			printf("child exiting..\n");
			sleep(1);
		}
	}
	else
	{
		perror("fork error\n");
	}
	return 0;
}
