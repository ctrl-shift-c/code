#include <thread_pool.h>

int thread_epoll_create(int max, int sockfd)
{
	int epfd = epoll_create(max);
	struct epoll_event temp;
	temp.data.fd = sockfd;
	temp.events = EPOLLIN|EPOLLET;
	epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &temp);
	return epfd;
}
