/* 多进程服务端模型， 可以完成1对多的连接与业务处理 */
#include <mysocket_api.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>


#define SERVER_SHUTDOWN 1

int main(void)
{
	struct sockaddr_in clientaddr;
	int serverfd, clientfd;
	socklen_t addrsize;
	serverfd = NET_INIT(0, 0, "192.168.102.100", 8000);
	char ip[16];
	char buffer[1500];
	bzero(buffer, sizeof(buffer));
	bzero(ip, sizeof(ip));
	int recvlen;
	int j;
	while(SERVER_SHUTDOWN)
	{
		//要记得每次都更改，初始化，该值可能被修改
		addrsize = sizeof(clientaddr);
		if((clientfd = ACCEPT(serverfd,(struct sockaddr *) &clientaddr, &addrsize))>0)
		{
			printf("server parent process %d client ip %s port %d\n",
				getpid(), inet_ntop(AF_INET, &clientaddr.sin_addr.sin_addr, ip, sizeof(ip)),
					 ntohs(clientaddr.sin_port));
			pid = fork();
			if(pid > 0)
			{
				printf("server parent process pid %d create pro successly..\n",getpid());
			}
			else if(pid == 0)
			{
				while((recvlen = recv(clientfd, buffer, sizeof(buffer),0))>0)
				{
					while(recvlen > j)
					{
						buffer[j] = toupper(buffer[j]);
						j++;
					}
					if((send(clientfd, buffer, recvlen, 0)==-1)
						STRERROR("send response msg error", 0);
					bzero(buffer, sizeof(buffer));
				}
				exit(0);
			}
			else
			{
				STRERROR("fork call failed", 0);
			}
		}
		
	}
	close(serverfd);
	return 0;
}
