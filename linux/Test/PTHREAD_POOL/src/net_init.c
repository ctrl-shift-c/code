#include <thread_pool.h>

int net_init(char *ip, int port)
{
	printf("ip %s, port %d\n", ip, port);
	struct sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &serveraddr.sin_addr.s_addr);
	
	int serverfd = SOCKET(AF_INET, SOCK_STREAM, 0);
	//int on = 1;
	//SETSOCKOPT(serverfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	BIND(serverfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	LISTEN(serverfd, 128);
	return serverfd;
}
