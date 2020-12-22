#include "./mysocket_api.h"
#include <sys/select.h>

#define SERVER_SHUTDOWN 1
int main(void)
{
	//变量
	struct sockaddr_in clientaddr;
	socklen_t addrlen;
	int readycode; //就绪量
	int maxfd; //最大socket
	int serverfd;
	int clientfd;
	char buffer[1500];
	int recvlen;
	int j;
	int client_array[1023];
	for(int i=0; i<1023; i++)
	{
		client_array[i] = -1;
	}
	bzero(buffer, sizeof(buffer));
	fd_set nset, oset; //nset监听集合， oset就绪集合
	FD_ZERO(&nset);
	//网络初始化
	serverfd = NET_INIT(SERVER_INIT, PROTOCOL_TCP, "192.168.102.100",8000);
	FD_SET(serverfd, &nset); //设置监听
	maxfd = serverfd;
	
	
	while(SERVER_SHUTDOWN)
	{
		oset = nset;
		readycode = select(maxfd + 1, &oset, NULL, NULL, NULL);
		// NULL 阻塞
		//设置非阻塞时需要判断
		//if(readycode <=0 )continue;
		if(FD_ISSET(serverfd, &oset))
		{//serverfd就绪
			addrlen = sizeof(clientaddr);
			printf("client accept\n");
			clientfd = ACCEPT(serverfd,(struct sockaddr *)&clientaddr, &addrlen);
			printf("clientfd %d \n", clientfd);
			if(maxfd < clientfd)
			{
				maxfd = clientfd;
			}
			for(int i=0; i<1023; i++)
			{
				if(client_array[i] == -1)
				{
					client_array[i] = clientfd;
					break;
				}
			}
			FD_SET(clientfd, &nset);
		}
		for(int i=0; i<1024; i++)
		{//clientfd就绪
			if(client_array[i] !=-1 && FD_ISSET(client_array[i], &oset))
			{
				if((recvlen = RECV(client_array[i], buffer, sizeof(buffer), 0))>0)
				{
					j=0;
					while(recvlen >j)
					{
						buffer[j] = toupper(buffer[j]);
						j++;
					}
					SEND(client_array[i], buffer, recvlen, 0);
					bzero(buffer, sizeof(buffer));
				}
				//检测客户端异常退出
				else
				{
					FD_CLR(client_array[i], &nset);
					close(client_array[i]);
					client_array[i] = -1;
					printf("client exiting ..\n");
				}
			}
		}
	}
}

