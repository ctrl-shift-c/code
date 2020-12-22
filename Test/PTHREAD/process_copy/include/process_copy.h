#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/wait.h>

int process_number;
int blocksize;
int temp_filesize;
int srcfd;
int desfd;
int nCount;

void process_copy_init(void);
void process_copy_check(int argc, char **argv);
void process_copy_cur(char *srcfile, char *desfile);
void process_copy(int index);
void process_copy_wait(pid_t *pidarray);

