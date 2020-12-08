#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void str_error(const char * errinfo, int exitcode)
{
	perror(errinfo);
	exit(exitcode);
}

void create_make(const char *make_path)
{
	int src_fd, des_fd;
	int read_size;
	char buf[0x1000];
	char old_path[1024];
	bzero(buf, sizeof(buf));
	bzero(old_path, sizeof(old_path));
	getcwd(old_path, sizeof(old_path));
	chdir(make_path);
	if((src_fd = open("Makefile", O_RDONLY))==-1)
		str_error("create_make error, open call failed",-1);
	chdir(old_path);	
	if((des_fd = open("Makefile", O_WRONLY|O_CREAT, 0664))==-1)
		str_error("create_make error, open call failed",-1);
	
	while((read_size = read(src_fd, buf, sizeof(buf)))>0)
		write(des_fd, buf, read_size);
	close(src_fd);
	close(des_fd);
}

int main()
{
	create_make("/home/colin/linux/Makefile/project/src");
	return 0;
}
