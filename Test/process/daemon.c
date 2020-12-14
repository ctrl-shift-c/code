#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

void* jobs(void *arg)
{
	printf("STDIN/STDOUT_FILENO close..\n");
	perror("STDERR dup2 error.log");
	int fd;
	time_t t;
	char timebuf[1024];
	if( (fd = open("datetime.log", O_RDWR|O_CREAT, 0664)) < 0)
	{
		perror("open datetime.log failed");
	}

	while(1)
	{
		bzero(timebuf, sizeof(timebuf));
		t = time(NULL);
		ctime_r(&t, timebuf);
		int size;
		if((size = write(fd, timebuf, strlen(timebuf)) )<= 0)
		{
			perror("datatime.log not ..");
			exit(0);
		}
		char buf[2048];
		sprintf(buf ,"write = %d", size);
		perror(buf);
		sleep(3);
	}
	close(fd);
	return (void *)0;
}

int main(void)
{
	pid_t pid;
	pid = fork();
	if(pid > 0)
	{
		printf("father exit..\n");
		exit(0);
	}
	else if(0 == pid)
	{
		int fd;
		int du;
		if( (fd = open("error.log", O_RDWR|O_CREAT, 0664)) < 0)
		{
			printf("open failed..\n");
		}
		printf("pid = [%d] sid = [%d]\n",getpid(), getsid(getpid()));
		setsid();
		printf("pid = [%d] sid = [%d]\n", getpid(), getsid(getpid()));
		chdir("./");
		printf("goto /\n");
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		if( (du = dup2(fd, STDERR_FILENO)) < 0)
		{
			printf("du2 failed\n");
		}
		umask(0);
		jobs(NULL);
		close(fd);
	}
	else
	{
		printf("fork failed..\n");
	}
}
