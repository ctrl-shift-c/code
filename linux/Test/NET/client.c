#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

int main(void)
{
	struct sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(8000);
	inet_pton(AF_INET, "192.168.102.100", &serveraddr.sin_addr.s_addr);

	int clientfd = socket(AF_INET, SOCK_STREAM, 0);
	int len;
	char buffer[1024] = {0};
	if( (connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) == 0)
	{ 
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
	return 0;
}

