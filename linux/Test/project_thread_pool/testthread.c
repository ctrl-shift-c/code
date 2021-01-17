#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

void* jobs(void*arg)
{
	
}
int main(void)
{
	int flags = 0;
	while(1)
	{
		pthread_t tid;
		if(pthread_create(&tid, NULL, jobs, NULL) >0)
			exit(0);
		printf("flags = %d\n", flags++);
	}
}
