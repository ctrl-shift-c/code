#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	if(argc < 3)
	{
		perror("fail argc < 3");
		exit(0);
	}
	kill(atoi(argv[2]), atoi(argv[1]));
	return 0;
}
