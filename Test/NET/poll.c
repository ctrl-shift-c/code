#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#include <poll.h>

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
		printf("setsockopt SOL_SOCK REUSEADDR failed\n");
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
	
	sigset_t sig;
	sigemptyset(&sig);
	sigaddset(&sig, SIGPIPE);
	sigprocmask(SIG_SETMASK, &sig, NULL);

	int i;
	struct pollfd serclientfd_array[4096];
	bzero(serclientfd_array, sizeof(serclientfd_array));
	int time; 
	for(i=0; i<4096; i++)
	{
		serclientfd_array[i].fd = -1;
		serclientfd_array[i].events = POLLOUT;
	}
	serclientfd_array[0].fd = serverfd;
	serclientfd_array[0].events = POLLIN;
	time = 5;

	struct sockaddr_in clientaddr;
	socklen_t clientlen;

	while(1)
	{
		int po = poll(serclientfd_array, 4096, -1);
		if(-1 == po)
		{
			printf("poll failed\n");
			exit(-1);
		}
		if(POLLIN == serclientfd_array[0].revents)
		{
			clientlen = sizeof(clientaddr);
	        bzero(&clientaddr, clientlen);
			int serclientfd = accept(serverfd, (struct sockaddr *)&clientaddr, &clientlen);
			if(-1 == serclientfd)
			{
				printf("accept failed\n");
				exit(-1);
			}
			
			char ip[16];
			inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, 16);
			printf("client port=[%d]\nclient ip=[%s]\n", ntohs(clientaddr.sin_port), ip);

			for(i=1; i<4096; i++)
			{
				if(serclientfd_array[i].fd == -1)
				{
					serclientfd_array[i].fd = serclientfd;
					break;
				}
			}
		}
		for(i=1; i<4096; i++)
		{
			if(serclientfd_array[i].fd != -1)
			{
				int sen = send(serclientfd_array[i].fd, "我就一直发", strlen("我就一直发"), 0);
				if(-1 == sen)
				{
					printf("cliend quit\n");
					serclientfd_array[i].fd = -1;
				}
				sleep(1);
			}
		}
	}
}
