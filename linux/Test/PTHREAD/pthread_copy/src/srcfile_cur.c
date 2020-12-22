#include <pthread_copy.h>
//切片，创建共享map映射，将源文件数据放入通道
int srcfile_cur(char *srcfile)
{
	int fd;
	if( (fd = open(srcfile, O_RDONLY)) <0 )
	{
		printf("srcfile open failed..\n");
		exit(-1);
	}
	filesize = lseek(fd, 0, SEEK_END);
	if(filesize % pthread_number)
		temp_filesize = filesize % pthread_number;
	blocksize = filesize / pthread_number;
	mmap_p = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
	return 0;
}
