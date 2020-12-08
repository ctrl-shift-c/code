#include <mysocket_api.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	int serverfd, clientfd;
	struct sockaddr_in serveraddr, clientaddr;
	serverfd = NET_INIT(1,1, "192.102.100", 8888);
	
	
}
