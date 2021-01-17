#include <thread_pool.h>

int thread_pool_init(int max, int min, int queuemax)
{	
	bzero(&thread_pool, sizeof(thread_pool));
	thread_pool.pool.shutdown = 1;
	thread_pool.pool.max = max;
	thread_pool.pool.min = min;
	thread_pool.queue.max = queuemax;
	thread_pool.queue.task = (task_t *)malloc(sizeof(task_t)*queuemax);
	if(thread_pool.queue.task  == NULL)
	{
		printf("malloc queue failed\n");
		return -1;
	}
	if(pthread_mutex_init(&thread_pool.mutex.queuelock, NULL) != 0 ||
			pthread_mutex_init(&thread_pool.mutex.acceptlock, NULL) != 0 ||
			pthread_mutex_init(&thread_pool.mutex.epolllock, NULL) != 0)
	{
		printf("lock init failed\n");
		return -1;
	}
	if(pthread_cond_init(&thread_pool.cont.producers, NULL) != 0 ||
			pthread_cond_init(&thread_pool.cont.consumers, NULL) != 0)
	{
		printf("cond init failed\n");
		return -1;
	}

	thread_pool.id.consumer_id = (pthread_t *)malloc(sizeof(pthread_t)*max);
	if(thread_pool.id.consumer_id == NULL)
	{
		printf("consumber malloc failed\n");
		return -1;
	}
	bzero(thread_pool.id.consumer_id, sizeof(pthread_t)*max);

	thread_pool.id.producer_id = (pthread_t *)malloc(sizeof(pthread_t)*10);
	if(thread_pool.id.producer_id == NULL)
	{
		printf("producer malloc failed\n");
		return -1;
	}
	bzero(thread_pool.id.producer_id, sizeof(pthread_t)*10);

	thread_pool.pool.attr = (pthread_attr_t *)malloc(sizeof(pthread_attr_t));
	if(thread_pool.pool.attr == NULL)
	{
		printf("thread_pool.pool.attr failed");
		return -1;
	}
	pthread_attr_init(thread_pool.pool.attr);
	pthread_attr_setdetachstate(thread_pool.pool.attr, PTHREAD_CREATE_DETACHED);

	int err;
	for(int i=0; i<min; i++)
	{
		err = pthread_create(&thread_pool.id.consumer_id[i], thread_pool.pool.attr, thread_consumer_jobs, NULL);
		printf("thread_pool.id.consumer_id = [0x%x]\n", (unsigned int)thread_pool.id.consumer_id[i]);
		if(err >0 )
		{
			printf("pthread create thread_consumer_jobs failed\n");
			return -1;
		}
		++thread_pool.pool.alive;
	}

	thread_pool.id.manager_id = (pthread_t *)malloc(sizeof(pthread_t));
	if(thread_pool.id.manager_id == NULL)
	{
		printf("manager malloc failed\n");
		return -1;
	}

	thread_pool.id.sigset_id = (pthread_t *)malloc(sizeof(pthread_t));
	if(thread_pool.id.sigset_id == NULL)
	{
		printf("manager malloc failed\n");
		return -1;
	}

	err = pthread_create(thread_pool.id.sigset_id, thread_pool.pool.attr, thread_pool_sigset_jobs, NULL);
	if( err >0 )
	{
		printf("thread_sigset_jobs create failed\n");
		return -1;
	}


    err = pthread_create(thread_pool.id.manager_id, thread_pool.pool.attr, thread_pool_manager_jobs, NULL);
	if(err >0 )
	{
	   printf("thread_manager_jobs create failed\n");
	   return -1;
	}
	return 1;
}
