#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MASG "asdasdsad"

int main(void)
{
	int fd[2];
	if(pipe(fd) < 0)
	{
		perror("create pipe fail\n");
		exit(0);
	}
	pid_t pid;
	pid = fork();
	if(pid > 0)
	{
		close(fd[0]);
		write(fd[1], MASG, strlen(MASG));
		printf("write %d\n", getpid());
		close(fd[1]);
		while(1);
	}
	else if(pid == 0)
	{
		close(fd[1]);
		char buffer[0x1000];
		bzero(buffer, strlen(buffer));
		read(fd[0], buffer, strlen(buffer));
		printf("read %s %d\n", MASG, getpid());
		close(fd[0]);
		while(1);
	}
	else
	{
		perror("create pid fail\n");
		exit(0);
	}
	return 0;
}
