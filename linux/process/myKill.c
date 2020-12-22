#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	if(argc < 3)
	{
		printf(" please pid..\n");
		exit(0);
	}
	kill(atoi(argv[2]), atoi(argv[1]));
	return 0;
}
