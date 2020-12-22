#include <pthread_copy.h>

int main(int argc, char **argv)
{
	file_check(argc, argv);
	srcfile_cur(argv[1]);
	desfile_copy(argv[2]);
	return 0;
}
