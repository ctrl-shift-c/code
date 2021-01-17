#include <thread_pool.h>
#define EPOLLMAX 200000

void* thread_socketbusiness(void *arg)
{
	int readycode;
	task_t task;
	int flags;
	while(thread_pool.pool.shutdown)
	{
		SYSTEM_ERROR(
			(readycode = epoll_wait(epfd, readyarray, EPOLLMAX, -1) >0) ,
			"epoll wait failed" ,
			SHUTUP
		);
		flags = 0;
		while(readycode > flags)
		{
			task.jobs = thread_clientbusiness;
			task.arg =(void *)&readyarray[flags++].data.fd;
			producer_jobs(&task);
		}
	}
	return NULL;
}
