#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>

void sigaction_chld(int arg)
{
	int err;
	while(waitpid(-1, &err, WNOHANG) >0)
	{
		if(WIFEXITED(err))
		{
			printf("process exit [%d]\n", WEXITSTATUS(err));
		}

		if(WIFSIGNALED(err))
		{
			printf("kill signal [%d]\n", WTERMSIG(err));
		}
	}
}

void process_jobs()
{
	sleep(2);
	exit(0);
}

void *pthread_jobs_joinprocess(void *arg)
{
	sigset_t sig;
	sigemptyset(&sig);
	sigprocmask(SIG_SETMASK, &sig, NULL);

	struct sigaction act;
	bzero(&act, sizeof(act));
	act.sa_handler = sigaction_chld;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaction(SIGCHLD, &act, NULL);
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
		perror("socket failed");
		exit(-1);
	}
	
	socklen_t on;
	int status = setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if(status == -1)
	{
		perror("setsockopt so_reuseraddr failed\n");
		exit(-1);
	}
	
	int bin = bind(serverfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if(-1 == bin)
	{
		perror("bind failed\n");
		exit(-1);
	}
	
	int lis = listen(serverfd, 128);
	if(-1 == lis)
	{
		perror("lis failed\n");
		exit(-1);
	}
	
	struct sockaddr_in clientaddr;
	socklen_t clientlen;
	
	sigset_t sig;
	sigemptyset(&sig);
	sigaddset(&sig, SIGCHLD);
	sigprocmask(SIG_SETMASK, &sig, NULL);

	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	while(1)
	{
		clientlen = sizeof(clientaddr);
		int acc = accept(serverfd, (struct sockaddr *)&clientaddr, &clientlen);
		if(acc == -1)
		{
			perror("accept failed\n");
			exit(-1);
		}
		pid_t pid;
		pid = fork();
		if(pid > 0)
		{
			char ip[16];
			inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, 16);
			printf("clientaddr port[%d]\nclientaddr ip[%s]\n",
					ntohs(clientaddr.sin_port), ip);
			pthread_create(&tid, &attr, pthread_jobs_joinprocess, NULL);
		}
		else if(pid == 0)
		{
			process_jobs();			
		}
		else
		{
			perror("fork failed\n");
			exit(-1);
		}

	}
	pthread_attr_destroy(&attr);
	
}

