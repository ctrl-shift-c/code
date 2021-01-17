#include <thread_pool_server.h>
#include <mysocket_api.h>

thread_pool_t* thread_pool_create(int max, int min, int quemax)
{
	thread_pool_t *ptr = NULL;
	if((ptr = (thread_pool_t *)malloc(sizeof(thread_pool_t)))==NULL)
	{
		STRERROR("malloc ptr call failed", 0);
		return NULL;
	}
	ptr->thread_shutdown = 1;
	ptr->thread_max = max;
	ptr->thread_min = min;
	ptr->thread_alive = 0;
	ptr->thread_busy = 0;
	ptr->thread_exitcode = 0;
	if((ptr->queue_task = (task_t *)malloc(sizeof(task_t)*quemax)) == NULL)
	{
		STRERROR("malloc queue_task call failed", 0);
		return NULL;
	}
	ptr->queue_front = 0;
	ptr->queue_rear = 0;
	ptr->queue_cur = 0;
	ptr->queue_max = quemax;
	if((ptr->customer_id = (pthread_t *)malloc(sizeof(pthread_t)*max)) == NULL)
	{
		STRERROR("malloc customer_id call faild", 0);
		return NULL;
	}
	

	bzero(ptr->customer_id, sizeof(pthread_t)*max);
	if((ptr->producer_id = (pthread_t *)malloc(sizeof(pthread_t)*10)) == NULL)
	{
		STRERROR("malloc producer call faild", 0);
		return NULL;
	}
	bzero(ptr->producer_id, sizeof(pthread_t)*10);
	if(pthread_mutex_init(&acclock, NULL)!=0 || pthread_mutex_init(&ptr->queuelock, NULL)!=0 || pthread_mutex_init(&epolllock, NULL)!=0 || pthread_cond_init(&ptr->not_full, NULL)!=0 || pthread_cond_init(&ptr->not_empty, NULL)!=0)
	{
		printf("lock or cond init error\n");
		return NULL;
	}

	int err;
	for(int i=0; i<min; i++)
	{
		if((err = pthread_create(&(ptr->customer_id[i]), NULL, thread_customer_jobs, (void *)ptr))>0)
		{
			STRERROR("thread_pool_create customer  call failed",err);
			return NULL;
		}
		++ptr->thread_alive;
	}

	if((err = pthread_create(&(ptr->manager_id), NULL, thread_manager_jobs, (void *)ptr))>0)
	{
		STRERROR("thread_pool_create manager call failed",err);
		return NULL;
	}
	printf("thread pool create  busy = %d\n", ptr->thread_busy);
	printf("[3] thread pool create successly..\n");
	return ptr;
}
