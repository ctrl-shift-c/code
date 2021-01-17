#include <thread_pool_server.h>
#include <mysocket_api.h>

void* thread_manager_jobs(void *arg)
{
	thread_pool_t *ptr = (thread_pool_t *)arg;
	int flags, add;
	while(ptr->thread_shutdown)
	{
		double alive, busy, cur;
		int err;
		pthread_mutex_lock(&ptr->queuelock);
		alive = ptr->thread_alive;
		busy = ptr->thread_busy;
		cur = ptr->queue_cur;
		pthread_mutex_unlock(&ptr->queuelock);
		if((alive - busy <= cur || busy / alive * 100 >= 70) && alive + ptr->thread_min <= ptr->thread_max)
		{
			for(flags =0, add=0; flags<ptr->thread_max && add<ptr->thread_min; flags++)
			{
				if(ptr->customer_id[flags]==0 || !thread_test_alive(ptr->customer_id[flags]))
				{
					if((err = pthread_create(&ptr->customer_id[flags], NULL, thread_customer_jobs, (void*)ptr))>0)
					{
						STRERROR("thread_manager_jobs add thread call failed",err);
						pthread_exit(NULL);
					}
					add++;
					pthread_mutex_lock(&ptr->queuelock);
					++(ptr->thread_alive);
					pthread_mutex_unlock(&ptr->queuelock);
				}
			}
		}
		if(busy * 2 <= alive - busy && alive - ptr->thread_min >= ptr->thread_min)
		{
			printf("exitcode .. %f %f %d\n", busy, alive, ptr->thread_min);
			pthread_mutex_lock(&ptr->queuelock);
			ptr->thread_exitcode = ptr->thread_min;
			pthread_mutex_unlock(&ptr->queuelock);
			for(flags = 0; flags < ptr->thread_min; flags++)
			{
				pthread_cond_signal(&ptr->not_empty);
			}	 
		}
		sleep(3);
	}
	pthread_exit(NULL);
}
