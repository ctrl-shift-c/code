#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

int pthread_number;
int filesize;
int blocksize;
char* mmap_p;
pthread_mutex_t lock;
int temp_filesize;
int desfd;
int nCount;

int file_check(int argc, char **argv);
int srcfile_cur(char *srcfile);
int desfile_copy(char *desfile);
