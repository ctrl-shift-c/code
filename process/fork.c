#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>



int main(void)
{
	pid_t pid;
	int i = 0;
	for(i; i<5; i++)
	{
		pid = fork();
		if(pid == 0)
			break;
	}
	if(pid > 0)
	{
		printf("running...1 pid [%d] gid [%d]\n", getpid(), getpgrp());
		while(1);
	}else if(pid == 0)
	{
		if(i == 3)
		{
			pid_t pigg;
			printf("parent pid [%d] \n", getppid());
			pigg = getppid();
			printf("pig [%d]\n", pigg);
			if(setpgid(pigg, getpid())==-1)
			{
				perror("faile");
			}
			printf("create child pid [%d] gid [%d]\n",getpid(),getpgrp());
			setpgid(getpid(), getpid());
			printf("create child pid [%d] gid [%d]\n",getpid(),getpgrp());
		}
		printf("running... pid [%d] gid [%d] %d\n",getpid(),getpgrp(), i);
	}else
	{
		perror("perror");
	}
	return 0;
}
