#include "./mysocket.h"

int main(void)
{
	struct sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(8000);
	inet_pton(AF_INET, "192.168.102.100", &serveraddr.sin_addr.s_addr);

	int serverfd = SOCKET(AF_INET, SOCK_STREAM, 0);
	int on = 1;
	SETSOCKOPT(serverfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	BIND(serverfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	LISTEN(serverfd, 128);	
	int serclientfd = ACCEPT(serverfd, NULL, NULL);
	SEND(serclientfd, "123", strlen("123"), 0);
	while(1);
}
