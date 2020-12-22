#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>

typedef struct 
{
	char str[1024];
	int id;
}CON;

int main(void)
{
	int wfd;
	if((wfd = open("mmapfile", O_RDWR|O_CREAT, 0664)) <0)
	{
		perror("open mmapfile failed..");
	}
	ftruncate(wfd, sizeof(CON));
	CON *p;
	if((p = mmap(NULL, sizeof(CON), PROT_WRITE, MAP_SHARED, wfd, 0)) == MAP_FAILED)
	{
		perror("mmap create failed..");
	}
	int i=0;
	close(wfd);
	while(1)
	{
		bzero(p, sizeof(CON));
		p->id = i;
		sprintf(p->str, "id=%d", i++);
		printf("%s %d\n", p->str, p->id);
		sleep(1);
	}
	munmap(p, sizeof(CON));
	return 0;
}
