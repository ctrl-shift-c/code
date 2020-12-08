#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

void *jobs(void *arg)
{
	time_t t;
	int fd;
	char timeBuf[1024];
	fd = open("localtime.log", O_RDWR|O_APPEND|O_CREAT,0664);
	while(1)
	{
		bzero(timeBuf, sizeof(timeBuf));
		t = time(NULL);
		ctime_r(&t, timeBuf);
		write(fd, timeBuf, strlen(timeBuf));
		sleep(10);
	}
	close(fd);
	return NULL;
}

void create_daemon_process(void)
{
	pid_t pid;
	pid = fork();
	if(pid > 0)
	{
		exit(0);
	}
	else if(pid == 0)
	{
		setsid();
		chdir("../");
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		umask(0);
		jobs(NULL);
	}
}

int main(void)
{
	create_daemon_process();
	return 0;
}
