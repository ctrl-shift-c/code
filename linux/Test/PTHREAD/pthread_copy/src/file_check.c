#include <pthread_copy.h>
//检查源文件是否存在，初始化， 得到线程数量
int file_check(int argc, char **argv)
{
	//-----------------测试参数是否正确----------------
	if(argc < 3)
	{
		printf("argc number not right ...\n");
		exit(-1);
	}
	//---------------------------------------------------
	//-------------------测试文件是否存在--------------
	if(access(argv[1], F_OK) < 0)
	{
		printf("srcfile not ...\n");
		exit(-1);
	}
	//------------------------------------------------
	//--------------------初始化-------------------
	pthread_number = 0;
	filesize = 0;
	blocksize = 0;
	temp_filesize = 0;
	desfd = 0;
	nCount = 0;
	mmap_p = NULL;
	pthread_mutex_init(&lock, NULL);
	//---------------------------------------------
	//--------------------得到线程数量-----------------
	if(argc == 4)
		pthread_number = atoi(argv[3]);
	if(argc == 3 || pthread_number < 5)
		pthread_number = 5;
	else if(pthread_number > 100)
	{
		printf("pthread_number not more <100 \n");
		exit(-1);
	}
	//------------------------------------------------
	return 0;
}
