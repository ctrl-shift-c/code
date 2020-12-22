#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MASG "ASdasdasd"

int main(void)
{
	int fd;
	fd = open("fifo", O_WRONLY);
	write(fd, MASG, strlen(MASG));
	close(fd);
	return 0;
}
