#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

typedef struct
{
	int pos;
	int filefd;
	int blocksize;
	int flags;
}thread_arg;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
char *pstr = NULL;
int temp;
void *thread_copy(void *arg)
{
	int len;
	thread_arg *arg1 = (thread_arg *)arg;
	pthread_mutex_lock(&lock);
	lseek(arg1->filefd, arg1->pos, SEEK_SET);
	if(arg1->flags)
		arg1->blocksize += temp;
	len = write(arg1->filefd, pstr+(arg1->pos), arg1->blocksize);
	pthread_mutex_unlock(&lock);
	printf("copy thread tid 0x%x copy pos %d blocksize %d write len %d\n", (unsigned int)pthread_self(), arg1->pos, arg1->blocksize, len);
	free(arg1);
	pthread_exit(NULL);
}

int arg_test(const char *arg01, const char *arg02)
{
	int thread_number;
	if((access(arg01, F_OK)) != 0)
	{
		printf("file is exited.\n");
		exit(0);
	}
	if(arg02 == 0)
	{
		thread_number = 5;
	}
	else
	{
		thread_number = atoi(arg02);
		if(thread_number <=0 && thread_number >=100)
		{
			printf("enter thread number error..\n");
			exit(0);
		}
	}
	return thread_number;
}

int block_cur(const char *files, int thread_number)
{
	int sfd = 0;
	int filesize = 0;
	sfd = open(files, O_RDONLY);
	filesize = lseek(sfd, 0, SEEK_END);
	if((filesize % thread_number))
	{
		temp = filesize % thread_number;
	}
	return filesize / thread_number;
}

int thread_create(const char *srcfile, const char *desfile, int thread_number, int blocksize)
{
	thread_arg *arg = NULL;
	int sfd = open(srcfile, O_RDONLY);
	int filesize = lseek(sfd, 0, SEEK_END);
	int dfd = open(desfile, O_RDWR|O_CREAT, 0664);
	int i=0;
	pstr = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, sfd, 0);
	close(sfd);
	pthread_t tid[thread_number];
	for(i;i<thread_number; i++)
	{
		arg = (thread_arg *)malloc(sizeof(thread_arg));
		bzero(arg, sizeof(thread_arg));
		arg->pos = i*blocksize;
		arg->blocksize = blocksize;
		arg->filefd = dfd;
		arg->flags = 0;
		if(temp>0 && i+1 == thread_number)
		{
			arg->flags = 1;
		}
		pthread_create(&tid[i], NULL, thread_copy, (void *)arg);
	}
	while(thread_number--)
		if(pthread_join(tid[thread_number], NULL) == 0)
			printf("thread join success tid [0x%x]\n", (unsigned int)tid[thread_number]);
	munmap(pstr, filesize);
	close(dfd);
}

int main(int argc, char **argv)
{
	int thread_number = 0;
	int blocksize = 0;
	if(argc < 3)
	{
		printf("Please enter secfile and desfile.\n");
		exit(0);
	}
	thread_number = arg_test(argv[1], argv[3]);
	blocksize = block_cur(argv[1], thread_number);
	thread_create(argv[1], argv[2], thread_number, blocksize);
	return 0;
}

