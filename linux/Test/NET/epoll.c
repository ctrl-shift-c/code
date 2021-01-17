#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#define MESSAGE "我就一直发"

int main(void)
{
	struct sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(8000);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == serverfd)
	{
		printf("socket failed\n");
		exit(-1);
	}
	
	int on = 1;
	int status = setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if(-1 == status)
	{
		printf("setsockopt failed\n");
		exit(-1);
	}

	int bin = bind(serverfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if(-1 == bin)
	{
		printf("bind failed\n");
		exit(-1);
	}

	int lis = listen(serverfd, 128);
	if(-1 == lis)
	{
		printf("listen failed\n");
		exit(-1);
	}

	struct epoll_event ready_array[200000];
	
	int epfd = epoll_create(200000);
	if(-1 == epfd)
	{
		printf("epoll_create failed\n");
		exit(-1);
	}

	struct epoll_event temp;
	temp.data.fd = serverfd;
	temp.events = EPOLLIN;
	epoll_ctl(epfd, EPOLL_CTL_ADD, serverfd, &temp);

	struct sockaddr_in clientaddr;
    socklen_t clientlen;

	sigset_t sig;
	sigemptyset(&sig);
	sigaddset(&sig, SIGPIPE);
	sigprocmask(SIG_SETMASK, &sig, NULL);

	while(1)
	{
		int readycode = epoll_wait(epfd, ready_array, 200000, -1);
		if(-1 == readycode)
		{
			printf("epoll_wait failed\n");
			exit(-1);
		}
		int nCount = 0;
		while(readycode > nCount)
		{
			printf("%d\n %d\n", readycode, nCount);
			if(serverfd == ready_array[nCount].data.fd)
			{
				clientlen = sizeof(clientaddr);
				int serclientfd = accept(serverfd, (struct sockaddr *)&clientaddr, &clientlen);
				if(-1 == serclientfd)
				{
					printf("accept failed\n");
					exit(-1);
				}

				char ip[16];
				inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, 16);
				printf("port=[%d]\nip=[%s]\n", ntohs(clientaddr.sin_port), ip);

				temp.data.fd = serclientfd;
				temp.events = EPOLLOUT|EPOLLET;
				epoll_ctl(epfd, EPOLL_CTL_ADD, serclientfd, &temp);
			}
			else
			{
				int sen =1;
				printf("serclientfd %d\n", ready_array[nCount].data.fd);
				//int sen = send(ready_array[nCount].data.fd, MESSAGE, strlen(MESSAGE), 0);
				if(-1 == sen)
				{
					printf("client quit\n");
					close(ready_array[nCount].data.fd);
					epoll_ctl(epfd, EPOLL_CTL_DEL, ready_array[nCount].data.fd, NULL);
				}
				printf("asd\n");
				sleep(1);
			}
			nCount++;
		}
	}
}
