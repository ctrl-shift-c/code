#include <thread_pool.h>

int thread_epoll_init(int epollmax)
{
	epfd = epoll_create(epollmax);

	USER_ERROR(
		((readyarray = (struct epoll_event *)malloc(sizeof(struct epoll_event)*epollmax)) != NULL) ,
		"readyarray create failed",
		SHUTUP
	);
	bzero(readyarray, sizeof(struct epoll_event)*epollmax);
	return 1;
}
