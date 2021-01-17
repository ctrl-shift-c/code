#include <thread_pool.h>
#define MESSAGE "连接成功"

void *BUSINESS_ACCEPT(void *arg)
{
	int serverfd = *(int *)arg;
	struct sockaddr_in clientaddr;
	bzero(&clientaddr, sizeof(clientaddr));
	socklen_t clientlen;
	clientlen = sizeof(clientaddr);
	pthread_mutex_lock(&thread_pool.mutex.acceptlock);
	int serclientfd = ACCEPT(serverfd, (struct sockaddr *)&clientaddr, &clientlen);
	pthread_mutex_unlock(&thread_pool.mutex.acceptlock);
	char ip[16];
	inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, 16);
	printf("client ip=[%s]\nclient port=[%d]\n", ip, ntohs(clientaddr.sin_port));

	struct epoll_event temp;
	temp.data.fd = serclientfd;
	temp.events = EPOLLIN|EPOLLET;
	pthread_mutex_lock(&thread_pool.mutex.epolllock);
	epoll_ctl(epfd, EPOLL_CTL_ADD, serclientfd, &temp);
	pthread_mutex_unlock(&thread_pool.mutex.epolllock);
	printf("BUSSINESS_ACCEPT successly\n");
	return NULL;
}
