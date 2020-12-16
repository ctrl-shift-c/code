#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* pthread_jobs(void *arg)
{
	printf("asf\n");
}

int main(void)
{
	pthread_t tid;
	pthread_attr_t attr;
	void *stackattr;
	size_t size;
	int ostate;
	int nstate;
	pthread_attr_init(&attr);
	pthread_attr_getdetachstate(&attr, &ostate);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_attr_getdetachstate(&attr, &nstate);
	pthread_attr_getstack(&attr, &stackattr, &size);
	printf("n = %d, o = %d\n", nstate, ostate);
	pthread_create(&tid, &attr, pthread_jobs,NULL);
	pthread_join(tid, NULL);
	printf("%d\n size = %ld\n", getpid(), size);
	while(1);
}
