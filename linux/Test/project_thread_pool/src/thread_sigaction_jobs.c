#include <thread_pool.h>

void sigaction_jobs(int arg)
{
	printf("sigaction %d\n", arg);
}

void *thread_sigaction_jobs(void *arg)
{
	int sign =(long)arg;
	struct sigaction sig;
	bzero(&sig , sizeof(sig));
	sig.sa_handler = sigaction_jobs;
	sig.sa_flags = 0;
	sigemptyset(&sig.sa_mask);
	sigaction(sign, &sig, NULL);
	while(1)
		sleep(1);
	return NULL;
}
