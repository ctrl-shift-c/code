#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MESSAGE "MESSAGE:mkfifo(char *filename, 0664)"

int main(void)
{
	int wfd;
	if(	(wfd = open("fifo", O_WRONLY))<0 )
	{
		perror("open file failed..");
	}
	write(wfd, MESSAGE, sizeof(MESSAGE));
	return 0;
}
