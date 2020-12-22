#include <thread_pool_server.h>

int thread_test_alive(pthread_t pid)
{
	int err;
	err = pthread_kill(pid, 0);
	if(err == ESRCH)
		return 0;
	return 1;
}
