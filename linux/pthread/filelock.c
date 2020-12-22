#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	int fd = open("b.c",O_RDWR|O_CREAT, 0664);
	struct flock lockattr;
	printf("%d %d\n" , lockattr.l_type ,F_WRLCK | F_RDLCK | F_UNLCK);

	fcntl(fd, F_GETLK, &lockattr);
	if(lockattr.l_type & F_WRLCK)
		printf("F_WRLCK\n");
	if(lockattr.l_type & F_RDLCK)
		printf("F_RDLCK\n");
	if(lockattr.l_type & F_UNLCK)
		printf("F_UNLCK\n");
	if(lockattr.l_type == F_WRLCK | F_RDLCK)
		printf("F_WRLCK, F_RDLCK, \n");
	if(lockattr.l_type == F_WRLCK | F_RDLCK | F_UNLCK)
		printf("F_WRLCK, F_RDLCK, F_UNLCK, \n");
	if(lockattr.l_type & F_UNLCK)
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
