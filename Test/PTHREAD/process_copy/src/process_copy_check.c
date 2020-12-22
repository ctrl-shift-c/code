#include <process_copy.h>

void process_copy_check(int argc, char **argv)
{
	if(argc < 3)
	{
		printf("argc number not right\n");
		exit(-1);
	}
	if( access(argv[1], F_OK)<0 )
	{
		printf("srcfile check failed...\n");
		exit(-1);
	}
	if(argc == 3)
		process_number = 5;
	else
	{
		process_number = atoi(argv[3]);
		if(process_number < 5)
			process_number = 5;
		if(process_number > 100)
		{
			printf("process_number not more 100...\n");
			exit(-1);
		}
	}
}
