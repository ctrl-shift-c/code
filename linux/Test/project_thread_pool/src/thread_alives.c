#include <thread_pool.h>

void* thread_alives(void *arg)
{
	while(thread_pool.pool.shutdown)
	{
		pthread_mutex_lock(&thread_pool.lock.connumberlock);
		int begincon = thread_pool.pool.begincon;
		pthread_mutex_unlock(&thread_pool.lock.connumberlock);

		pthread_mutex_lock(&thread_pool.lock.pronumberlock);
        int beginpro = thread_pool.pool.beginpro;
        pthread_mutex_unlock(&thread_pool.lock.pronumberlock);
//		printf("begincon %d beginpro %d\n", begincon, beginpro);
		while(begincon--)
		{
//			printf("123\n");
			pthread_kill(thread_pool.tid.sigaction[0], 35);
		}
		while(beginpro--)
		{
			pthread_kill(thread_pool.tid.sigaction[1], 34);
		}
		sleep(1);
	}
	return  NULL;
}
