#include <thread_pool.h>
#define EPOLLMAX 200000

int main(void)
{
	thread_epoll_init(200000);
	thread_pool_init(1000, 10, 1000);
	while(thread_pool.pool.shutdown)
	{
		thread_socketbusiness(NULL);
	}
	return 0;
}
