#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MESSAGE "恭喜你连接成功"


int main(void)
{
	struct sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(8000);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int serverfd = socket(AF_INET, SOCK_STREAM, 0);

	bind(serverfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

	listen(serverfd, 128);

	int seclientfd;
	seclientfd = accept(serverfd, NULL, NULL);
	send(seclientfd, MESSAGE, strlen(MESSAGE), 0);
	int recvlen;
	char rebuffer[1024];
	bzero(rebuffer, sizeof(rebuffer));
	while((recvlen = recv(seclientfd, rebuffer, sizeof(rebuffer), 0)) >0 )
	{
		printf("%s\n", rebuffer);
	}


	return 0;
}
