#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(void)
{
	int fd;
	fd = open("map_file", O_RDWR);
	int filesize = lseek(fd, 0, SEEK_END);
	int *p;
	if((p = mmap(NULL, filesize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0))
			 == MAP_FAILED)
	{
		perror("mmap error\n");
		exit(0);
	}
	close(fd);
	p[0]=0x34333231;
	return 0;
}

