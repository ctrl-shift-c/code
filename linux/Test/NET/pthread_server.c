#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>

#define MESSAGE "我就一直发"

void sigaction_jobs(int arg)
{
	printf("cliend exit\n");
}


void* pthread_jobs(void *arg)
{
	
	sigset_t set;
	sigemptyset(&set);
	sigprocmask(SIG_SETMASK, &set, NULL);
	
	int serclientfd = (long int)arg;
	while(1)
	{
		if(send(serclientfd, MESSAGE, strlen(MESSAGE), 0) <= 0)
		{
			printf("client quit\n");
			break;
		}
		else
			sleep(2);
	}
	close(serclientfd);
	pthread_exit(NULL);
}

void* pthread_jobs_join(void *arg)
{
	struct sigaction act;
	bzero(&act, sizeof(act));
	act.sa_handler = SIG_IGN;//sigaction_jobs;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaction(SIGPIPE, &act, NULL);
	while(1)
		sleep(1);
	pthread_exit(NULL);
}

int main(void)
{
	struct sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(8000);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if(serverfd == -1)
	{
		printf("socket create failed\n");
		exit(-1);
	}

	int bin = bind(serverfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if(bin == -1)
	{
		printf("bind failed\n");
		exit(-1);
	}

	int lis = listen(serverfd, 128);
	if(lis == -1)
	{
		printf("listem failed\n");
		exit(-1);
	}

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	/*
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGPIPE);
	//sigprocmask(SIG_SETMASK, &set, NULL);
	pthread_sigmask(SIG_BLOCK, &set, NULL);
	*/
	
	struct sockaddr_in clientaddr;
	int clisize;
	pthread_t tid;
	pthread_create(&tid, &attr, pthread_jobs_join, NULL);
	while(1)
	{
		bzero(&clientaddr, sizeof(clientaddr));
		clisize = sizeof(clientaddr);
		int serclientfd = accept(serverfd, (struct sockaddr *)&clientaddr, &clisize);
		if(serclientfd == -1)
		{
			printf("accept failed\n");
			exit(-1);
		}
		printf("%d\n", serclientfd);
		pthread_create(&tid, &attr, pthread_jobs, (void *)(long)serclientfd);
	}
	pthread_attr_destroy(&attr);
	close(serverfd);
}
