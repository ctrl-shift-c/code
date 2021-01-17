#include "./mysocket.h"
#define TCP_PORT 8000
#define TCP_IP "192.168.102.100"

int main(void)
{
	struct sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(TCP_PORT);
	inet_pton(AF_INET, TCP_IP, &serveraddr.sin_addr.s_addr);
	int clientfd = SOCKET(AF_INET, SOCK_STREAM, 0);
	CONNECT(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	char buffer[1024] = {0};
	RECV(clientfd, buffer, sizeof(buffer), 0);
	printf("%s\n", buffer);
	while(1);
}
