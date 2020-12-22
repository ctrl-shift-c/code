#include <process_copy.h>


void process_copy(int index)
{
	int pos = index * blocksize;
	lseek(desfd, pos, SEEK_SET);
	lseek(srcfd, pos, SEEK_SET);
	int readlen;
	char buffer[blocksize+1];
	bzero(buffer, sizeof(buffer));
	if ( (readlen = read(srcfd, buffer, blocksize)) >0 )
		write(desfd, buffer, blocksize);
	else
	{
		printf("read failed...\n");
		exit(-1);
	}
	if(index == process_number-1 && temp_filesize)
	{
		bzero(buffer, sizeof(buffer));
		lseek(desfd, pos+blocksize, SEEK_SET);
		lseek(srcfd, pos+blocksize, SEEK_SET);
	    if ( (readlen = read(srcfd, buffer, temp_filesize)) >0 )
		    write(desfd, buffer, temp_filesize);
		else
		{
			printf("read failed...\n");
			exit(-1);
		}
	}
}
