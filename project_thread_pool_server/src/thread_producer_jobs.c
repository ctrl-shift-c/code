#include <thread_pool_server.h>
#include <mysocket_api.h>

int thread_producer_jobs(thread_pool_t *ptr, task_t task)
{
	pthread_mutex_lock(&ptr->queuelock);
	if(ptr->thread_shutdown)
	{
		while(ptr->queue_max == ptr->queue_cur)
		{
			pthread_cond_wait(&ptr->not_full, &ptr->queuelock);
		}
		if(!(ptr->thread_shutdown))
		{
			pthread_mutex_unlock(&ptr->queuelock);
			printf("producer thread 0x%x exiting..\n",(unsigned int)pthread_self());
			pthread_exit(NULL);
		}
		if(BUSINESS_A == task.jobs)
			printf("BUSINESS_A ..\n");
		else
			printf("BUSINESS_B ..\n");
		ptr->queue_task[ptr->queue_front].jobs = task.jobs;
		ptr->queue_task[ptr->queue_front].arg = task.arg;
		ptr->queue_front = (ptr->queue_front + 1) % ptr->queue_max;
		++ptr->queue_cur;
		pthread_cond_signal(&ptr->not_empty);	
	}
	pthread_mutex_unlock(&ptr->queuelock);
	return 0;
}
