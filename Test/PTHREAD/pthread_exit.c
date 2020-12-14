#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
void *pthread_job(void *arg)
{
	printf("pthread4 [0x%x]\n", (unsigned int)pthread_self());
	while(1);
}

void* pthread_job1(void *arg)
{
	printf("pthread1 [0x%x]\n", (unsigned int)pthread_self());
	pthread_exit((void *)1);
	//exit(0);
}

void* pthread_job2(void *arg)
{
	printf("pthread2 [0x%x]\n", (unsigned int)pthread_self());
	return (void *)2;
}

void* pthread_job3(void *arg)
{
	printf("pthread3 [0x%x]\n", (unsigned int)pthread_self());
	sleep(15);
	exit(0);
}


int main(void)
{
	void *reval;
	pthread_t tid[4];
	printf("pthread0 [0x%x]\n", (unsigned int)pthread_self());
	pthread_create(tid+0, NULL, pthread_job1, NULL);
	printf("pthread1 [0x%x]\n", (unsigned int)tid[0]);
	sleep(1);
	//pthread_join(tid[0], &reval);
	printf("reval = %d\n", *(int *)reval);
	pthread_create(tid+1, NULL, pthread_job2, NULL);
	printf("pthread2 [0x%x]\n", (unsigned int)tid[1]);
	sleep(1);
	pthread_join(tid[1], &reval);
	printf("reval = %d\n", *(int *)reval);
	pthread_create(tid+2, NULL, pthread_job3, NULL);
	printf("pthread3 [0x%x]\n", (unsigned int)tid[2]);
	sleep(1);
	pthread_create(tid+3, NULL, pthread_job, NULL);
	printf("pthread4 [0x%x]\n", (unsigned int)tid[3]);
	sleep(1);
	pthread_cancel(tid[3]);
	sleep(1);
	while(1);
}

