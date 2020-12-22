#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#define FLAGS 5000

typedef struct
{
	int code;
	pthread_mutex_t lock;
}data_t;

int main(void)
{
	int fd = open("process_mmap", O_RDWR|O_CREAT, 0664);
	ftruncate(fd ,sizeof(data_t));
	data_t *ptr = mmap(NULL, sizeof(data_t), PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
	close(fd);
	
	pthread_mutexattr_t lock_attr;
	pthread_mutexattr_init(&lock_attr);
	pthread_mutexattr_setpshared(&lock_attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&ptr->lock, &lock_attr);
	ptr->code = 0;
	int temp;
	pid_t pid;
	pid = fork();
	if(pid > 0)
	{
		for(int i=0; i<FLAGS; i++)
		{
			pthread_mutex_lock(&ptr->lock);
			temp = ptr->code;
			printf("parent peocess id %d ++flag %d\n", getpid(), ++temp);
			ptr->code = temp;
			pthread_mutex_unlock(&ptr->lock);
		}
	}
	else if(pid == 0)
	{
		for(int i=0; i<FLAGS; i++)
		{
			pthread_mutex_lock(&ptr->lock);
			temp = ptr->code;
			printf("child peocess id %d ++flag %d\n", getpid(), ++temp);
			ptr->code = temp;
			pthread_mutex_unlock(&ptr->lock);
		}
	}
	else
	{
		perror("fork call fail");
	}
	wait(NULL);
	pthread_mutexattr_destroy(&lock_attr);
	pthread_mutex_destroy(&ptr->lock);
	munmap(ptr, sizeof(data_t));
	return 0;
}
