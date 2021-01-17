#include <thread_pool.h>

int thread_pool_destroy()
{
	for(int i=0; i<thread_pool.pool.max; i++)
	{
		if( thread_pool.id.consumer_id[i] )
		{
			pthread_cancel(thread_pool.id.consumer_id[i]);
			printf("cancel 0x%x\n", (unsigned int)thread_pool.id.consumer_id[i]);
		}
	}

	thread_pool.pool.shutdown = 0;
	pthread_mutex_destroy(&thread_pool.mutex.queuelock);
	pthread_mutex_destroy(&thread_pool.mutex.acceptlock);
	pthread_mutex_destroy(&thread_pool.mutex.epolllock);
	pthread_cond_destroy(&thread_pool.cont.producers);
	pthread_cond_destroy(&thread_pool.cont.consumers);

	free(thread_pool.queue.task);
	free(thread_pool.id.consumer_id);
	free(thread_pool.id.producer_id);
	free(thread_pool.id.manager_id);
	free(thread_pool.id.sigset_id);
	return 1;
}
