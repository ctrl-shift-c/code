#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


void *thread_1(void *arg)
{
	printf("thread_1 tid [0x%x] returning..\n", pthread_self());
	return (void *)1;
}

void *thread_2(void *arg)
{
	printf("thread_2 tid [0x%x] returning..\n", pthread_self());
	return (void *)2;
}

void *thread_3(void *arg)
{
	printf("thread_3 tid [0x%x] returning..\n", pthread_self());
	return (void *)3;
}

int main(void)
{
	void *reval = NULL;
	pthread_t tid;
	pthread_create(&tid, NULL, thread_1, NULL);
	pthread_join(tid, &reval);
	printf("main ")
	
}
