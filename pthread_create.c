#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void * thread_job(void *arg)
{
	while(1)
	{
		printf("thread create success. runing...\n");
		sleep(1);
	}
	return NULL;
}

int main(void)
{
	pthread_t tid;
	pthread_create(&tid, NULL, thread_job, NULL);
	printf("main thread create success tid [%x]\n",(unsigned int)tid);
	while(1)
		sleep(1);	

	return 0;
}
