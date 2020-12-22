#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

#define MESSAGE "father to child"
int main(void)
{
	pid_t pid;
	int fd[2];
	if(pipe(fd)<0)
	{
		perror("pipe create failed..\n");
		exit(0);
	}
	printf("pipe create successly..\n");
	pid = fork();
	if(pid>0)
	{
		close(fd[0]);
		write(fd[1], MESSAGE, strlen(MESSAGE));
		close(fd[1]);
	}
	else if(pid == 0)
	{
		close(fd[1]);
		char buffer[1024];
		bzero(buffer, sizeof(buffer));
		while(read(fd[0], buffer, sizeof(buffer)))
		{
			printf("read %s\n", buffer);
			bzero(buffer, sizeof(buffer));
		}
		close(fd[0]);
	}
	else
	{
		printf("fork failed..\n");
	}
	return 0;
}
