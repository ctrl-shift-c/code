#include <thread_pool.h>
#define MESSAGE "我就一直发"

void *BUSINESS_SEND(void *arg)
{
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	int serclientfd = *(int *)arg;
	char buffer[1024] = {0};
	int rec = RECV(serclientfd, buffer, sizeof(buffer) , 0);
	int sen = SEND(serclientfd, MESSAGE, strlen(MESSAGE), 0);
	if(sen == -1)
	{
		close(serclientfd);
		epoll_ctl(epfd, EPOLL_CTL_DEL, serclientfd, NULL);
		printf("client quit %d\n", serclientfd);
	}
	/*
	else
	{
		struct epoll_event temp;
		temp.data.fd = serclientfd;
		temp.events = EPOLLOUT|EPOLLET;
		epoll_ctl(epfd, EPOLL_CTL_MOD, serclientfd, &temp);
	}
	*/
	sleep(20);
	printf("BUSINESS_SEND quit\n");
	return NULL;
}
