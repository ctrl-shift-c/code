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
	if((fd = open("./errorfile.log", O_RDWR|O_CREAT,0664))<0)
	{
		printf("open failed..\n");
	}
	int size = lseek(fd, 0, SEEK_END);
	printf("%d\n", size);
	lseek(fd, size, SEEK_SET);
	if((dupfd = dup2(fd, STDOUT_FILENO))<0)
	{
		printf("dup failed..\n");
	}
	printf("aaa\n");
	write(STDOUT_FILENO, "asd\n",4);
	close(fd);
	close(dupfd);
	return 0;
}
