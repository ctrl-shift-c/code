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
	fd = open("map_file", O_RDWR);
	ginfo_t *p = NULL;
	if((p = mmap(NULL, sizeof(ginfo_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0))
			 == MAP_FAILED)
	{
		perror("mmap error\n");
		exit(0);
	}
	while(1)
	{
		printf("RECV: G[%s] V[%d] P[%d]\n", p->game_name, p->version, p->price);
		sleep(1);
	}
	close(fd);
	munmap(p, sizeof(ginfo_t));
	return 0;
}

