#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	int flags = 0;
	alarm(1);
	while(1)
		printf("asd %d\t", ++flags);
	return 0;
}
