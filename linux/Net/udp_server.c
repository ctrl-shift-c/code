#include <mysocket_api.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	int serverfd;
	struct sockaddr_in clientaddr;
	char buffer[1500];
	char ip[16];
	int recvlen;
	bzero(buffer, sizeof(buffer));
	bzero(ip, sizeof(ip));
	serverfd = NET_INIT(0, 1, "192.168.102.100", 8888);
	socklen_t addrsize = sizeof(clientaddr);
	if((recvlen = RECVFROM(serverfd,buffer,sizeof(buffer),0,(struct sockaddr *)&clientaddr, &addrsize))>0)
	{
		printf("UDP SERVER RECV SUCCESS client ip %s client port %d client msg:%s\n", 
				inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, 16), ntohs(clientaddr.sin_port), buffer);
	}
	close(serverfd);
	return 0;
}
