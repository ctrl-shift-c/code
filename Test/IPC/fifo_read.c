#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void)
{
	int rfd;
	if(	(rfd = open("fifo", O_RDONLY))<0 )
	{
		perror("open file failed..");
	}
	char buffer[1024] = {0};
	while( read(rfd, buffer, sizeof(buffer)))
		printf("%s..\n", buffer);
	return 0;
}
