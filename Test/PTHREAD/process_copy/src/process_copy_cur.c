#include <process_copy.h>


void process_copy_cur(char *srcfile, char *desfile)
{
	if( (srcfd = open(srcfile, O_RDONLY) ) <0 )
	{
		printf("open srcfile failed...\n");
		exit(-1);
	}
	if( (desfd = open(desfile, O_WRONLY|O_CREAT, 0664)) <0 )
	{
		printf("desfd failed...\n");
		exit(-1);
	}
	int filesize = lseek(srcfd, 0, SEEK_END);
	if(filesize % process_number)
		temp_filesize = filesize % process_number;
	blocksize = filesize / process_number;
}
