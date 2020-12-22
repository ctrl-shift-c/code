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


int main(void)
{
	//网络socket
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

	socklen_t on = 1;
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


	struct sockaddr_in clientaddr;
	socklen_t clientlen;

	//屏蔽pipe
	sigset_t sig;
	sigemptyset(&sig);
	sigaddset(&sig, SIGPIPE);
	sigprocmask(SIG_SETMASK, &sig, NULL);

	//select
	fd_set nrset, orset, nwset, owset;
	int maxfd = serverfd;
	struct timeval mytime;
	mytime.tv_sec = 2;
	mytime.tv_usec = 200;

	FD_ZERO(&nrset);
	FD_ZERO(&nwset);
	FD_SET(serverfd, &nrset);

	int i;
	int serclient_array[1024];
	memset(serclient_array, -1, sizeof(serclient_array));

	while(1)
	{
		orset = nrset;
		owset = nwset;

		int sel = select(maxfd+1, &orset, &owset, NULL, &mytime);
		if(0 == sel)continue;
		if(-1 == sel)
		{
			printf("select failed\n");
			exit(-1);
		}

		if(FD_ISSET(serverfd, &orset))
		{
			bzero(&clientaddr, sizeof(clientaddr));
			clientlen = sizeof(clientaddr);
			int serclientfd = accept(serverfd, (struct sockaddr *)&clientaddr, &clientlen);
			if(-1 == serclientfd)
			{
				printf("accept failed\n");
				exit(-1);
			}
			
			char ip[16];
			inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, 16);
			printf("client port = [%d]\nip=[%s]\n", ntohs(clientaddr.sin_port), ip);

			if(serclientfd > 1024)
			{
				printf("select 1024 full, not add\n");
			}
			else
			{
				if(serclientfd > maxfd)
				{
					maxfd = serclientfd;	
				}
				for(i=4;i<1024; i++)
				{
					if(-1 == serclient_array[i])
					{
						serclient_array[i] = serclientfd;
						break;
					}
				}
				FD_SET(serclientfd, &nwset);
			}
		}
		for(i=4; i<1024; i++)
		{
			if(serclient_array[i] != -1)
			{
				int sen = send(serclient_array[i], "我就一直发", strlen("我就一直发"), 0);
				if(sen == -1)
				{
					printf("client quit\n");
					FD_CLR(serclient_array[i], &nwset);
					close(serclient_array[i]);
					serclient_array[i] = -1;
				}
				sleep(1);
			}
		}
	}
}
