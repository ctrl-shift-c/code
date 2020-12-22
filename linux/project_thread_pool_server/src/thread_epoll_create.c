#include <thread_pool_server.h>
#include <mysocket_api.h>
int thread_epoll_create(int maxsize, int sockfd)
{
	int epfd;
	struct epoll_event temp;
	if((epfd = epoll_create(maxsize))==-1)
	{
		STRERROR("epoll create call failed", 0);
		return -1;
	}
	temp.data.fd = sockfd;
	temp.events = EPOLLIN;
	temp.events |= EPOLLET;
	if((epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &temp))==-1)
	{
		STRERROR("epoll ctl add call failed", 0);
		return -1;
	}
	printf("[2] epoll initalizer successly epfd (%d)\n",epfd);
	return epfd;
}
