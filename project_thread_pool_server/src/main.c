#include <mysocket_api.h>
#include <thread_pool_server.h>


int main(void)
{
	int serverfd, flags;
	int readycode;
	struct epoll_event ready_array[_EPOLL_MAXSIZE_];
	task_t temp;
	//1.网络初始化
	serverfd = NET_INIT(SERVER_INIT, PROTOCOL_TCP, "192.168.102.100", 8000);
	//2.epoll初始化
	epfd = thread_epoll_create(_EPOLL_MAXSIZE_, serverfd);
	//3.线程池创建
	thread_pool_t *ptr = NULL;
	ptr = thread_pool_create(300, 10, 500);
	//4. 事件监听
	while(ptr->thread_shutdown)
	{
		readycode = epoll_wait(epfd, ready_array, _EPOLL_MAXSIZE_, -1);
		flags = 0;
		while(readycode)
		{
			if(ready_array[flags].data.fd == serverfd)
			{
				printf("BUSINESS_A mian\n");
				temp.jobs = BUSINESS_A;
				temp.arg = (void *)&ready_array[flags].data.fd;
				thread_producer_jobs(ptr, temp);
			}
			else
			{
				printf("BUSINESS_B main\n");
				temp.jobs = BUSINESS_B;
				temp.arg = (void *)&ready_array[flags].data.fd;
				thread_producer_jobs(ptr, temp);
			}
			++flags;
			--readycode;
		}
	}
	close(serverfd);
	exit(0);	
}
