#include <thread_pool_server.h>
#include <mysocket_api.h>

void *BUSINESS_A(void *arg)
{
	struct epoll_event temp;
	int sockfd = *(int *)arg;
	int cliendfd;
	struct sockaddr_in clientaddr;
	socklen_t addrlen = sizeof(clientaddr);
	pthread_mutex_lock(&acclock);
	cliendfd = ACCEPT(sockfd, (struct sockaddr *)&clientaddr, &addrlen);
	pthread_mutex_unlock(&acclock);
	printf("customer thread 0x%x BUSINESS A , client accept successly..\n", (unsigned int)pthread_self());
	if(cliendfd > 0)
	{
		temp.data.fd = cliendfd;
		temp.events = EPOLLIN;
		temp.events |= EPOLLET;
		pthread_mutex_lock(&epolllock);
		epoll_ctl(epfd, EPOLL_CTL_ADD, cliendfd, &temp);
		pthread_mutex_unlock(&epolllock);
	}
	return NULL;
}
