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
	int rfd;
	if((rfd = open("mmapfile", O_RDONLY)) <0)
	{
		perror("open mmapfile failed..");
	}
	CON *p;
	if((p = mmap(NULL, sizeof(CON), PROT_READ, MAP_SHARED, rfd, 0)) == MAP_FAILED)
	{
		perror("mmap create failed..");
	}
	close(rfd);
	while(1)
	{
		printf("%s %d\n", p->str, p->id);
		sleep(1);
	}
	munmap(p, sizeof(CON));
	return 0;
}
