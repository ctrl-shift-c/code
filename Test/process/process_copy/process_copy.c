#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int temp;

int arg_test(const char *arg01, const char *arg03)
{
	int process_number = 5;
	if((access(arg01, F_OK))!=0)
	{
		printf("src file is exited.\n");
		exit(0);
	}
	if(*arg03 < 5)
		process_number = 5;
	else if(*arg03 < 100)
	{
		process_number = atoi(arg03);	
	}
	else
	{
		printf("process number not create..\n");
		exit(0);
	}
	return process_number;
}

void process_create(const char *srcfile, const char *desfile, const int process_number, const int blocksize)
{
	int srcfd = open(srcfile, O_RDONLY);
	int filesize = lseek(srcfd, 0, SEEK_END);
	int desfd = open(desfile, O_RDWR|O_CREAT, 0664);
	int i=0;
	pid_t pid, pidArray[process_number];
	printf("##:#    #    #    #    #0%%\n");
	for(i=0;i<process_number;i++)
	{
		pid = fork();
		if(pid == 0)break;
		pidArray[i] = pid;
	}
	if(pid > 0)
	{
		int number = process_number;
		//printf("create  child process success..\n");
		if(temp)
		{
			lseek(desfd, blocksize*process_number, SEEK_SET);
			lseek(srcfd, blocksize*process_number, SEEK_SET);
			char buffer[temp];
			if(read(srcfd, buffer, sizeof(buffer)))
			{
				write(desfd, buffer, temp);
			}
		}
		while(number)
		{
			for(int j=0; j<process_number; j++)
			{
				if(waitpid(pidArray[j], NULL, WNOHANG)>0)
				{
					//printf("return child pid=%d..\n", pidArray[j]);
					number--;
					int mod = (process_number-number)/(double)process_number*100;
					if(mod == 25)
					{
						printf("##:######    #    #    #25%%\n");
					}
					else if(mod == 50)
					{
						printf("##:###########    #    #50%%\n");
					}
					else if(mod == 75)
					{
						printf("##:################    #75%%\n");
					}
					else if(mod == 100)
					{
						printf("##:#####################100%%\n");
					}
				}
			}
			usleep(1000);
		}
		close(srcfd);
		close(desfd);
		exit(0);
	}
	else if(pid == 0)
	{
		int len;
		char buffer[blocksize];
		int read_size = 0;
		bzero(buffer, sizeof(buffer));
		lseek(desfd, i*blocksize, SEEK_SET);
		lseek(srcfd, i*blocksize, SEEK_SET);
		if(read_size = read(srcfd, buffer, sizeof(buffer)))
		{
			write(desfd, buffer, read_size);
		}
	}
	else
	{
		printf("fork fail..\n");
	}
}

int block_cur(const char *srcfile, int process_number)
{
	int filefd;
	int filesize = 0;
	int process_size;
	filefd = open(srcfile, O_RDONLY);
	filesize = lseek(filefd, 0, SEEK_END);
	process_size = filesize/process_number;
	if(filesize%process_number)
	{
		temp = filesize%process_number;
	}
	return process_size;
}

int main(int argc, char **argv)
{
	int process_number = 0;
	int blocksize = 0;
	if(argc < 3)
	{
		printf("date error..\n");
		exit(0);
	}
	process_number = arg_test(argv[1],argv[3]);
	blocksize = block_cur(argv[1], process_number);
//	printf("temp = %d blocksize = %d\n", temp, blocksize);
	process_create(argv[1], argv[2], process_number, blocksize);
}
