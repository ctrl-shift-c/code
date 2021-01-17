#include <thread_pool.h>

void *thread_consumer_jobs(void *arg)
{
	while(thread_pool.pool.shutdown)
	{
		pthread_mutex_lock(&thread_pool.lock.queuelock);
		while(0 == thread_pool.queue.cur)
		{
			pthread_cond_wait(&thread_pool.cond.cond_consumer, &thread_pool.lock.queuelock);
			if(thread_pool.pool.conexitcode > 0)
			{
				pthread_mutex_unlock(&thread_pool.lock.queuelock);
				pthread_mutex_lock(&thread_pool.lock.connumberlock);
				if(thread_pool.pool.conexitcode > 0)
				{
					--thread_pool.pool.conexitcode;
					--thread_pool.pool.connumber;
					pthread_cond_signal(&thread_pool.cond.cond_consumer);
			    	pthread_mutex_unlock(&thread_pool.lock.connumberlock);
				//	--kil;
//			        printf("manager begin kill consumer\n");
					pthread_kill(thread_pool.tid.sigaction[1], 35);
				//	--kil;
					pthread_exit(NULL);
				}
				else
				{
					pthread_mutex_unlock(&thread_pool.lock.connumberlock);
					continue;
				}
			}
			pthread_mutex_unlock(&thread_pool.lock.connumberlock);
		}
		--thread_pool.queue.cur;
		task_t task;
		int rear = thread_pool.queue.rear;
		thread_pool.queue.rear = (rear + 1)%thread_pool.queue.max;
		task.jobs = thread_pool.queue.task[rear].jobs;
		task.arg = thread_pool.queue.task[rear].arg;
		pthread_mutex_unlock(&thread_pool.lock.queuelock);

		pthread_mutex_lock(&thread_pool.lock.conbusylock);
        ++thread_pool.pool.conbusy;
        pthread_mutex_unlock(&thread_pool.lock.conbusylock);

		task.jobs(task.arg);

		pthread_mutex_lock(&thread_pool.lock.conbusylock);
		--thread_pool.pool.conbusy;
		pthread_cond_signal(&thread_pool.cond.cond_consumer);
		pthread_mutex_unlock(&thread_pool.lock.conbusylock);
	}
	pthread_exit(NULL);
}
