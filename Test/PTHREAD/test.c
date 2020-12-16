#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//三个普通线程以不同的退出方式退出结束， 主控线程回收
void *thread_1(void *arg)
{
	printf("thread_1 tid [0x%x] returning..\n",(unsigned int)pthread_self());
	sleep(20);
	return (void *)1;
}

void *thread_2(void *arg)
{
	printf("thread_1 tid [0x%x] exiting..\n", (unsigned int)pthread_self());
	sleep(20);
	pthread_exit((void *)2);
}

void *thread_3(void *arg)
{
	while(1)
	{
		printf("thread_1 tid [0x%x] exiting..\n", (unsigned int)pthread_self());
		sleep(1);
	}
}


int main(void)
{
	pthread_t tid;
	pthread_create(&tid, NULL, thread_1, NULL);
	void *reval = NULL;
	pthread_join(tid, &reval);
	printf("main thread tid [0x%x] join thread 1, return number %ld",
						(unsigned int)pthread_self(), (long int)reval);
	pthread_create(&tid, NULL ,thread_2, NULL);
	pthread_join(tid, &reval);
	printf("main thread tid [0x%x] join thread 1,exit code  %ld",
						(unsigned int)pthread_self(), (long int)reval);
	
	pthread_create(&tid, NULL ,thread_3, NULL);
	sleep(5);
	pthread_cancel(tid);
	pthread_join(tid, &reval);
	printf("main thread tid [0x%x] join thread 1,cancel code  %ld",
						(unsigned int)pthread_self(), (long int)reval);
	while(1);
}
