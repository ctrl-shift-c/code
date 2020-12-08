#include <mysocket_api.h>
#include <pthread.h>

#define SERVER_SHUTDOWN 1

void *thread_jobs(void *arg)
{
	BUSINESS_01(*(int *)arg);
}

int main(void)
{
	struct sockaddr_in clientaddr;
	int clientfd;
	int err;
	int serverfd = NET_INIT(SERVER_INIT, PROTOCOL_TCP, "192.168.102.100",8000);
	pthread_t tid;
	socklen_t addrsize;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	printf("thread server service runing..\n");
	while(SERVER_SHUTDOWN)
	{
		addrsize = sizeof(clientaddr);
		if((clientfd = ACCEPT(serverfd,(struct sockaddr *)&clientaddr, &addrsize))>0)
		{
			if((err == pthread_create(&tid, &attr, thread_jobs,(void *)&clientfd))>0)
				STRERROR("thread create error", err);
		}
	}
	pthread_attr_destroy(&attr);
	close(serverfd);
	close(clientfd);
}
