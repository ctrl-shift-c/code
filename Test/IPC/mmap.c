#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(void)
{
	int wfd;
	if((wfd = open("mmapfile", O_RDWR|O_CREAT, 0664)) <0 )
	{
		perror("open mmapfile failed..");
		exit(0);
	}
	int filesize = lseek(wfd, 0, SEEK_END);
	printf("%d\n",filesize);
	char *p;
	if((p = mmap(NULL, filesize, PROT_READ|PROT_WRITE, MAP_SHARED, wfd, 0)) == MAP_FAILED)
	{
		perror("mmap create failed..");
	}
	close(wfd);
	p[0]='1';
	return 0;
}
