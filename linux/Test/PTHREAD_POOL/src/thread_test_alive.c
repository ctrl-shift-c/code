#include <thread_pool.h>

int thread_test_alive(pthread_t tid)
{
	if(thread_pool.pool.shutdown)
	{
		int err = pthread_kill(tid , 0);
		if(ESRCH == err)
			return 0;
	}
	return 1;
}
