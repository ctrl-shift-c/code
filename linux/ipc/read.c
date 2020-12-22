#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	int fd;
	char buffer[0x1000];
	bzero(buffer, sizeof(buffer));
	fd = open("fifo", O_RDONLY);
	read(fd, buffer, sizeof(buffer));
	printf("read %s\n", buffer);
	close(fd);
	return 0;
}
