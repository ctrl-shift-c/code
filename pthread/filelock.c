#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	int fd = open("b.c",O_RDWR);
	struct flock lockattr;
	fcntl(fd, F_GETLK, &lockattr);
	printf("%d\n", lockattr.l_type);
	if(lockattr.l_type == F_WRLCK || lockattr.l_type == F_RDLCK || lockattr.l_type == F_UNLCK)
	{
		printf("b.c types [unlock].\n");
		lockattr.l_type = F_RDLCK;
		lockattr.l_whence = SEEK_SET;
		lockattr.l_start = 0;
		lockattr.l_len = 0;
		if(fcntl(fd, F_SETLK, &lockattr) == 0)
		{
			printf("b.c set file lock success [RDLOCK].\n");
		}
	}
		
	lockattr.l_type = F_UNLCK;
	lockattr.l_whence = SEEK_SET;
	lockattr.l_start = 0;
	lockattr.l_len = 0;
	if(fcntl(fd, F_SETLK, &lockattr) == 0)
	{
		printf("b.c set file lock success [UNLOCK].\n");
	}
	close(fd);
	return 0;
}
