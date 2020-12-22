#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


int main()
{
	pid_t pid;
	pid = fork();
	if(pid > 0)
	{
		printf("%d 1\n", getpid());
		while(1);
	}else if(pid == 0)
	{
		printf("%d 2\n", getpid());
		execl("/usr/bin/firefox", "firefox", "www.baidu.com", NULL);
		printf("不会被执行\n");
	}else
	{
		perror("fork call failed");
		exit(0);
	}	
}
