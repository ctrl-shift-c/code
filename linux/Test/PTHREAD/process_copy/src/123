#include <process_copy.h>

int main(int argc, char **argv)
{
	process_copy_init();
	process_copy_check(argc, argv);
	process_copy_cur(argv[1], argv[2]);
	pid_t pid;
	pid_t array[process_number];
	int i;
	for(i=0; i<process_number; i++)
	{
		pid = fork();
		if(pid == 0)break;
		array[i] = pid;
	}
	if(pid > 0)
	{
		process_copy_wait(array);	
	}
	else if(pid == 0)
	{
		process_copy(i);
	}
	else
	{
		printf("fork failed...\n");
		exit(-1);
	}
}
