#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
	alarm(1);
	pause();
	while(1)
	{
		printf("asd\n");
	}
}
