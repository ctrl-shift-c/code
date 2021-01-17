#include <thread_pool.h>

int thread_producer_jobs(task_t *jobs)
{
	task_t task;
	task.jobs = jobs->jobs;
	task.arg = jobs->arg;
	pthread_mutex_lock(&thread_pool.mutex.queuelock);
//	printf("1 producer lock\n");
	if(thread_pool.pool.shutdown)
	{
		while(thread_pool.queue.max == thread_pool.queue.cur)
		{
//			printf("1 producer unlock\n");
			int cond = pthread_cond_wait(&thread_pool.cont.producers, &thread_pool.mutex.queuelock);
//			printf("2 producer lock\n");
			if(cond > 0)
			{
				printf("cond wait failed\n");
				return -1;
			}
		}
		if(!(thread_pool.pool.shutdown))
		{
			pthread_mutex_unlock(&thread_pool.mutex.queuelock);
			printf("producer thread %d exiting\n", (unsigned int)pthread_self());
			pthread_exit(NULL);
		}
		if(BUSINESS_ACCEPT == task.jobs)
			printf("producer BUSINESS_ACEPT\n");
		else
			printf("producer BUSINESS_SEND\n");
		int front = thread_pool.queue.front;
		thread_pool.queue.front = (thread_pool.queue.front + 1) % thread_pool.queue.max;
		thread_pool.queue.cur++;
		thread_pool.queue.task[front].jobs = task.jobs;
		thread_pool.queue.task[front].arg = task.arg;
	}
	pthread_cond_signal(&thread_pool.cont.consumers);
	pthread_mutex_unlock(&thread_pool.mutex.queuelock);
//	printf("2 producer unlock\n");
	return 0;
}
