#include <stdio.h>
#include <stdlib.h>

int fun(int a, int b)
{
	return a+b;
}

int begin(void)
{
	int a,b;
	printf("test_start\n");
	scanf("%d%d",&a,&b);
	printf("%d\n", fun(a,b));
	exit(0);
}
