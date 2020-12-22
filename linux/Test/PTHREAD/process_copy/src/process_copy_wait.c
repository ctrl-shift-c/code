#include <process_copy.h>

void process_copy_wait(pid_t *pid)
{
	while(nCount)
	{
		int flag = waitpid(pid[1], NULL, WNOHANG);
		if(flag <0 )
		{
			printf("waitpid failed...\n");
			exit(-1);
		}
		if(flag >0 )
			nCount--;
	}
}
