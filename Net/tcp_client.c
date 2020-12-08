#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define SERVER_IP "192.168.102.100"
#define SERVER_PORT 8000

int main(void)
{
	//客户端初始化，clientaddr看需求设置，如果需要绑定就写，不需要就不用写
	struct sockaddr_in serveraddr, clientaddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_IP, &serveraddr.sin_addr.s_addr);
	//inet_pton将字符串的点序转化为整形
	//inet_pton(ip类型，字符串， 存储)
	//客户端是否需要绑定 bind
	bzero(&clientaddr, sizeof(clientaddr));
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_port = htons(9000);
	clientaddr.sin_addr.s_addr = htons(INADDR_ANY);

	int clientfd = socket(AF_INET, SOCK_STREAM, 0);
	bind(clientfd, (struct sockaddr *)&clientaddr, sizeof(clientaddr));
	char buffer[1500];
	int len;
	bzero(buffer, sizeof(buffer));
	//客户端发起请求
	if(connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == 0)
	{
		printf("client pid %d connect successly..\n", getpid());
		while(fgets(buffer, sizeof(buffer), stdin) != NULL)
		{
			//1. 发送请求
			send(clientfd, buffer, strlen(buffer), 0);
			//2. 获取响应
			len = recv(clientfd, buffer, sizeof(buffer), 0);
			//3. 显示结果
			printf("client recv response msg:%s\n", buffer);
		}
	}
	close(clientfd);
	return 0;
}
