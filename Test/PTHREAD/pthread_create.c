#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *jobs(void *arg)
{
	printf("asdf\n");
}

int main(void)
{
	pthread_t tid;
	pthread_create(&tid, NULL, jobs, NULL);
	printf("[0x%x]\n",(unsigned int)pthread_self());
	sleep(1);
	printf("%d\n", pthread_join(tid, NULL));
}
