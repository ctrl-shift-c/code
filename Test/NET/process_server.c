#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>

void sigaction_jobs(int arg)
{
	int status;
	while(waitpid(-1, &status, WNOHANG) > 0)
	{
		printf("waitpid\n");
		if(WIFEXITED(status))
		{
			printf("process code = [%d]\n", WEXITSTATUS(status));
		}
		if(WIFSIGNALED(status))
		{
			printf("kill process signal = [%d]\n", WTERMSIG(status));
		}
	}
}

void* pthread_jobs(void *arg)
{	
	struct sigaction sigac, osig;
	bzero(&sigac, sizeof(sigac));
	sigac.sa_handler = sigaction_jobs;
	sigac.sa_flags = 0;
	sigemptyset(&sigac.sa_mask);
	sigaction(SIGCHLD, &sigac, &osig);

	while(1)
		sleep(2);
	pthread_exit(NULL);
}
void childprocess_jobs(int serclientfd)
{
	/*
	sigset_t nsig;
	sigemptyset(&nsig);
	sigprocmask(SIG_SETMASK, nsig, NULL);
	*/
	while(1)
	{
		//读端关闭写端被杀死
		if((send(serclientfd, "MESSAGE", strlen("MESSAGE"), 0)) >0 )
			sleep(1);
	}
}

int main(void)
{
	/*
	sigset_t sig;
	sigemptyset(&sig);
	sigaddset(SIGCHLD, &sig);
	sigprocmask(SIG_SETMASK, &sig, NULL);
	*/

	struct sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(8000);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if(serverfd == -1)
		printf("socket failed\n");

	if(bind(serverfd,(struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
		printf("bind failed\n");

	if( listen(serverfd, 128) == -1)
		printf("listen failed\n");

	int serclientfd;
	pid_t pid;
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&tid, &attr, pthread_jobs, NULL);
	while(1)
	{
		struct sockaddr_in clientaddr;
		socklen_t clisize;	
		clisize = sizeof(clientaddr);
		if((serclientfd = accept(serverfd,(struct sockaddr *)&clientaddr, &clisize)) >0 )
		{
			pid = fork();
			if(pid > 0)
			{
				char ip[16] = {0};
				inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, 16);
				printf("father process = [0x%x]\nport = [%d]\nip = [%s]\n", getpid(),ntohs(clientaddr.sin_port), ip);
			}
			else if(pid == 0)
			{
				childprocess_jobs(serclientfd);
				//子进程被杀死，以后的代码都不会被执行
			}
			else
			{
				printf("fork failed\n");
				exit(-1);
			}
		}
		else
		{
			//printf("accept failed : %s \n", strerror(errno));
			if(errno == EINTR)
			{
				printf("client exit\n");
				continue;
			}
			else
			{
				exit(-1);
			}
		}
	}
	return 0;
}
