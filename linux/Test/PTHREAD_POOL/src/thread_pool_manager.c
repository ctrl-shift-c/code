#include <thread_pool.h>

void *thread_pool_manager_jobs(void *arg)
{
	while(thread_pool.pool.shutdown)
	{
		printf("manager\n");
		double alive, busy, cur;
		int min, max;
		pthread_mutex_lock(&thread_pool.mutex.queuelock);
		printf("mananer 1lock\n");
		alive = thread_pool.pool.alive;
		busy = thread_pool.pool.busy;
		cur = thread_pool.queue.cur;
		min = thread_pool.pool.min;
		max = thread_pool.pool.max;
		pthread_mutex_unlock(&thread_pool.mutex.queuelock);
//		printf("mananer 1unlock\n");
		//printf("alive %lf, busy %lf, cur %lf\n", alive, busy, cur);
		if(alive - busy <= cur || busy / alive * 100 >= 70)
		{
			int nflag = min;
			if(alive + nflag >= max);
			else
			{
				for(int i=0; i<max && nflag > 0; i++)
				{
					if(thread_pool.id.consumer_id[i] == 0 || !thread_test_alive(thread_pool.id.consumer_id[i]))
					{
						pthread_create(&thread_pool.id.consumer_id[i], thread_pool.pool.attr, thread_consumer_jobs, NULL);
						printf("consumer 0x%x\n", (unsigned int)thread_pool.id.consumer_id[i]);
						--nflag;
//						printf("mananer 2lock\n");
						pthread_mutex_lock(&thread_pool.mutex.queuelock);
						++thread_pool.pool.alive;
						pthread_mutex_unlock(&thread_pool.mutex.queuelock);
//						printf("manager 2unlock\n");
					}
				}
			}
		}
		else if(busy * 2 < alive)
		{
			if(alive - min < min);
			else
			{
				pthread_mutex_lock(&thread_pool.mutex.queuelock);
                thread_pool.pool.exitcode = min;
				pthread_mutex_unlock(&thread_pool.mutex.queuelock);

				while(min--)
				{
					pthread_cond_signal(&thread_pool.cont.consumers);
				}

				/*
				int can = min;
				for(int i=0; i<max && can > 0; i++)
				{
					if(thread_pool.id.consumer_id[i] != 0)
					{
//						pthread_cancel(thread_pool.id.consumer_id[i]);
//						printf("mananer 3lock\n");
//						printf("mananer 3unlock\n");
						can--;
					}
				}
				*/
			}
		}
		sleep(3);
		printf("thread_pool.pool.alive %d\nthread_pool.pool.busy %d\nthread_pool.queue.cur %d\n", thread_pool.pool.alive, thread_pool.pool.busy, thread_pool.queue.cur);
	}
	return NULL;
}
