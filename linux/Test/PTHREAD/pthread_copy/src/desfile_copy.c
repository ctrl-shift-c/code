#include <pthread_copy.h>

void *pthread_copy(void *arg)
{
	pthread_detach(pthread_self());

	int index = *(int *)arg;
	int pos = index * blocksize;
	int size = blocksize;
	if(index == pthread_number - 1)
	{
		size += temp_filesize;
	}
	pthread_mutex_lock(&lock);
	lseek(desfd, pos, SEEK_SET);
	int len = write(desfd, mmap_p+pos, size);
	pthread_mutex_unlock(&lock);
	if(len <= 0)
	{
		printf("write failed...\n");
		exit(-1);
	}
	nCount--;
	free(arg);
	pthread_exit(NULL);
}

int desfile_copy(char *desfile)
{
	if( (desfd = open(desfile, O_CREAT|O_RDWR, 0664)) <0 )
	{
		printf("desfile open failed...\n");
	}
	pthread_t tid;
	nCount = pthread_number;
	for(int i=0; i<pthread_number; i++)
	{
		int *index = (int *)malloc(sizeof(int));
		*index = i;
		pthread_create(&tid, NULL, pthread_copy, index);
	}
	while(nCount > 0);
	close(desfd);
	return 0;
}
