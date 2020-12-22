/* 具备网络连接能力服务器应用 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>

int main(void)
{

	//网络信息初始化
	struct sockaddr_in serveraddr, clientaddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(8000);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);	
	//创建服务端sockfd
	int serverfd = socket(AF_INET, SOCK_STREAM, 0);
	//自定义信息绑定socket
	bind(serverfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	//监听连接信息
	listen(serverfd, 128);
	//阻塞等待客户端连接
	int clientfd;
	socklen_t addrsize = sizeof(clientaddr);

	char ip[16];
	bzero(ip, 16);
	
	int recvlen;
	char recvbuffer[1500];
	bzero(recvbuffer, sizeof(recvbuffer));
	int j;
	if((clientfd = accept(serverfd, (struct sockaddr *)&clientaddr, &addrsize)) > 0)
	{
		printf("Server accept success , client_app ip:%s port:%d\n",
			 inet_ntop(AF_INET, &clientaddr.sin_addr, ip, 16), ntohs(clientaddr.sin_port));

		/*服务端业务处理*/
		//1. 读取客户端请求数据
		while((recvlen = recv(clientfd, recvbuffer, sizeof(recvbuffer), 0)) > 0 )
		{
			j = 0;
			while(recvlen > j)
			{
				recvbuffer[j] = toupper(recvbuffer[j]);
				// toupper小写转大写 #include <ctype.h>
				j++;
			}
			// 3. 反馈相应
			send(clientfd, recvbuffer, recvlen, 0);
			bzero(recvbuffer, sizeof(recvbuffer));
		}
	}
	close(clientfd);
	close(serverfd);
	return 0;
}
