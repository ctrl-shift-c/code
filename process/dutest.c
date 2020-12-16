#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;
	int dupfd;
	if((fd = open("./errorfile.log", O_RDWR|O_TRUNC|O_CREAT,0664))<0)
	{
		printf("open failed..\n");
	}
	if((dupfd = dup(fd))<0)
	{
		printf("dup failed..\n");
	}
	write(dupfd, "asd\n",4);
	close(fd);
	close(dupfd);
	return 0;
}
