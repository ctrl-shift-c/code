#include <thread_pool.h>

int thread_net_init(char *ip, int port)
{
	struct sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &serveraddr.sin_addr.s_addr);
	int serverfd = SOCKET(AF_INET, SOCK_STREAM, 0);
	int on = 1;
	SETSOCKOPT(serverfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	SETSOCKOPT(serverfd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
	int flags;
	SYSTEM_ERROR((flags = fcntl(serverfd, F_GETFL, 0)) !=-1,"fcntl F_GETFL: non block failed", SHUTUP);
	SYSTEM_ERROR(fcntl(serverfd, F_SETFL, flags|O_NONBLOCK) != -1, "fcntl F_SET: non block failed", SHUTUP);
	BIND(serverfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	LISTEN(serverfd, 5);
	return serverfd;
}
