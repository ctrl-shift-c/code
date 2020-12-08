#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


int main(void)
{
	void *stackaddr = NULL;
	size_t stacksize = 0;
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_getstack(&attr,&stackaddr,&stacksize);
	printf("DFL THREAD ATTR STACKADDR [%p] STACKSIZE [%d]\n", stackaddr,(int)stacksize);
	pthread_attr_destroy(&attr);
	return 0;
}
