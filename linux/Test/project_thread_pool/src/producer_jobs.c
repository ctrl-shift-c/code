#include <thread_pool.h>

int producer_jobs(task_t *task)
{
	if(thread_pool.pool.shutdown)
	{
		pthread_mutex_lock(&thread_pool.lock.queuelock);
		while(thread_pool.queue.cur == thread_pool.queue.max)
		{
			pthread_cond_wait(&thread_pool.cond.cond_producer, &thread_pool.lock.queuelock);
		}
		if(!thread_pool.pool.shutdown)
		{
			pthread_mutex_unlock(&thread_pool.lock.queuelock);
			printf("thread pool shutdown\n");
			return -1;
		}
		int front = thread_pool.queue.front;
		thread_pool.queue.task[front].jobs = task->jobs;
		thread_pool.queue.task[front].arg = task->arg;
		thread_pool.queue.front = (front+1) % thread_pool.queue.max;
		++thread_pool.queue.cur;
		pthread_cond_signal(&thread_pool.cond.cond_consumer);
		pthread_mutex_unlock(&thread_pool.lock.queuelock);
	}
	return 1;
}
