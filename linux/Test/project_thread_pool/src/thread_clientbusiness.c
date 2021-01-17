#include <thread_pool.h>
#define MESSAGE "我就一直发"

void* thread_clientbusiness(void *arg)
{
	int serclientfd = *(int *)arg;
	char buffer[1024] = {0};
	int rec = RECV(serclientfd, buffer, sizeof(buffer), 0);
	int sen = SEND(serclientfd, buffer, rec, 0);
	if(sen == -1)
	{
		epoll_ctl(epfd, EPOLL_CTL_DEL, serclientfd, NULL);
	}
//	printf("%d\n", nCount++);
	/*
	struct epoll_event temp;
	temp.data.fd = serclientfd;
	temp.events = EPOLLOUT|EPOLLET;
	epoll_ctl(epfd, EPOLL_CTL_MOD, serclientfd, &temp);
	*/
	sleep(1);
	return NULL;
}
