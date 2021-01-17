#include <thread_pool.h>

void sigaction_jobs(int arg)
{
	printf("sigpipe \n");
}

void *thread_pool_sigset_jobs(void *arg)
{
	printf("thead sigset jobs\n");
	struct sigaction act;
	bzero(&act, sizeof(act));
	act.sa_handler = sigaction_jobs;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaction(SIGPIPE, &act, NULL);
	while(thread_pool.pool.shutdown)
		sleep(1);
	pthread_exit(NULL);
}
