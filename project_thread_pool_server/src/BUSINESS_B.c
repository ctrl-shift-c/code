#include <thread_pool_server.h>
#include <mysocket_api.h>

void *BUSINESS_B(void *arg)
{
	struct epoll_event temp;
	int sockfd = *(int *)arg;
	int recvlen;
	int j;
	char buffer[1500];
	bzero(buffer, sizeof(buffer));
	printf("BUSINESS_B .. %d\n", sockfd);
	while((recvlen = RECV(sockfd, buffer, sizeof(buffer), 0))>0)
	{
		j=0;
		printf("BUSINESS_B %s\n", buffer);
		while(recvlen > j)
		{
			buffer[j] = toupper(buffer[j]);
			j++;
		}
		SEND(sockfd, buffer, recvlen, 0);
		bzero(buffer, sizeof(buffer));
		printf("customer thread exec 0x%x BUSINESS_B ..\n", (unsigned int)pthread_self());
	}
	printf("BUSINESS_B recvlen %d\n", recvlen);
	if(recvlen == 0)
	{
		pthread_mutex_lock(&epolllock);
		epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, NULL);
		pthread_mutex_unlock(&epolllock);
		close(sockfd);
		printf("customer thread 0x%x exec BUSINESS B client exit..\n",(unsigned int)pthread_self());
	}
	return NULL;
}
