#include <thread_pool.h>
#define IP "192.168.102.100"
#define PORT 8000
#define MAXEPOLL 200000

int main(void)
{
	int serverfd = net_init(IP, PORT);
	printf("[1] net_init successly\n");
	struct epoll_event ready_array[MAXEPOLL];
	SYSTEM_ERROR((epfd = thread_epoll_create(MAXEPOLL, serverfd)), "epoll create failed");
	printf("[2] epoll_create sucessly\n");
	USER_ERROR(thread_pool_init(200, 5, 300), "thead_pool_init failed");
	printf("[3] thread_pool_init successly\n");
	int nCount ;
	task_t task;
	int readycode;
	while(thread_pool.pool.shutdown)
	{
		nCount = 0;
		readycode = epoll_wait(epfd, ready_array, MAXEPOLL, -1);
		while(readycode > nCount)
		{
			if(ready_array[nCount].data.fd == serverfd)
			{
				task.jobs = BUSINESS_ACCEPT;
				task.arg = (void *)&serverfd;
				//printf("BUSINESS_ACCEPT main\n");
			}
			else
			{
				task.jobs = BUSINESS_SEND;
				task.arg = (void *)&ready_array[nCount].data.fd;
				//printf("BUSINESS_SEND main\n");
			}
			thread_producer_jobs(&task);
			++nCount;
		}
		sleep(1);
	}
	printf("thread_pool quit\n");
	close(serverfd);
	exit(0);
}
