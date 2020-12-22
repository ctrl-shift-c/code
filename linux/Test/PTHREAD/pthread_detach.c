#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *pthread_jobs(void *arg)
{
	pthread_detach(pthread_self());
	sleep(2);
}

int main(void)
{
	pthread_t tid;
	pthread_create(&tid, NULL, pthread_jobs, NULL);
	printf("pig == %d\n", getpid());
	while(1);
}
