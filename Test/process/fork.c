#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
	pid_t pid[5],temp;
	int i;
	for(i=0; i<5; i++)
	{
		temp = fork();
		if(temp == 0)break;
		pid[i] = temp;
	}
	if(temp > 0)
	{
		printf("father runing [%d]\n",getpid());
		while(1)
		{
			int flag;
			for(int j=0; j<5; j++)
				if(flag = waitpid(pid[i]/*-1*/, NULL, WNOHANG) >0)
				{
					printf("waitpid success..\n");
				}
		}
	}
	else if(temp == 0)
	{
		printf("child runing [father] = [%d] child = [%d]\n",getppid(), getpid());
		if(i!=3)
			while(1);
	}
	else
	{
		perror("fork error");
	}
}
