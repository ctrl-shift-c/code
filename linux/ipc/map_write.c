#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

typedef struct
{
	int version;
	char game_name[1024];
	int price;
}ginfo_t;

int main(void)
{
	int fd;
	fd = open("map_file", O_RDWR|O_CREAT, 0664);
	ftruncate(fd, sizeof(ginfo_t));
	ginfo_t *p;
	
	if((p = mmap(NULL, sizeof(ginfo_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0))
			 == MAP_FAILED)
	{
		perror("mmap error\n");
		exit(0);
	}
	p->version = 1;
	p->price = 298;
	while(1)
	{
		bzero(p->game_name, sizeof(p->game_name));
		sprintf(p->game_name, "AAA%d", ++(p->version));
		sleep(1);
	}
	close(fd);
	munmap(p, sizeof(ginfo_t));
	return 0;
}

