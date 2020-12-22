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
#include <sys/select.h>

#define MESSAGE "我就一直发消息"

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
		printf("socket failed\n");
		exit(-1);
	}

	int on;
	int status  = setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if(status == -1)
	{
		printf("setsockopt failed\n");
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
		printf("listen failed\n");
		exit(-1);
	}

	int client_array[1024];
	fd_set nrset, nwset, owset, orset;
	FD_ZERO(&nrset);
	FD_ZERO(&nwset);
	int i;
	for(i=0; i<1024; i++)
	{
		client_array[i] = -1;
	}
	FD_SET(serverfd, &nrset);
	int myfd = serverfd;
	int serclientfd;
	struct sockaddr_in clientaddr;
	socklen_t cliaddrlen;

	struct sigaction act;
	bzero(&act, sizeof(act));
	act.sa_handler = SIG_IGN;
	act.sa_flags = 0;
	sigaction(SIGPIPE, &act, NULL);

//	orset = nrset;
	while(1)
	{
		printf("while\n");
		owset = nwset;
		orset = nrset;
		struct timeval mytime;
		mytime.tv_sec = 27;
		mytime.tv_usec=0;
		int sel = select(myfd+1, &orset, &owset, NULL, &mytime);
		if(sel == -1)
		{
			printf("select failed\n");
			exit(-1);
		}
		if(sel == 0)continue;

		if(FD_ISSET(serverfd, &orset))
		{
			printf("serverfd\n");
			bzero(&clientaddr, sizeof(clientaddr));
			cliaddrlen = sizeof(clientaddr);
			serclientfd = accept(serverfd, (struct sockaddr *)&clientaddr, &cliaddrlen);
			printf("accept\n");
			if(serclientfd == -1)
			{
				printf("accept failed \n");
				exit(-1);
			}

			if(serclientfd > myfd)
				myfd = serclientfd;
			if(serclientfd == -1)
			{
				printf("accept failed\n");
				exit(-1);
			}
			
			for(i=0; i<1024; i++)
			{
				if(client_array[i] == -1)
				{
					client_array[i] = serclientfd;
					printf("serclinedfd = %d\n", client_array[i]);
					break;
				}
			}

			if(i == 1024)
				printf("select 1024 full , not add new serclientfd\n");
			else
			{
				FD_SET(serclientfd, &nwset);
				char ip[16] ={0};
				inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, 16);
				printf("port=[%d]\nip=[%s]\n", ntohs(clientaddr.sin_port), ip);
			}
		}
		for(i=0; i<1024; i++)
		{
			if(client_array[i] != -1)
			{
				printf("client_array %d\n", client_array[i]);
				if(send(client_array[i], MESSAGE, strlen(MESSAGE), 0) < 0)
				{
					FD_CLR(client_array[i], &nwset);
					printf("%d client quit\n", client_array[i]);
					client_array[i] = -1;
				}
				sleep(1);
			}
		}
	}
}
