#include <process_copy.h>

void process_copy_init(void)
{
	process_number = 0;
	blocksize = 0;
	srcfd = 0;
	desfd = 0;
	nCount = 0;
}
