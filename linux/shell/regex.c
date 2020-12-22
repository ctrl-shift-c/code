#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <regex.h>

int main(void)
{
	int fd;
	if((fd = open("date.log", O_RDONLY)) == -1)
	{
		perror("open file failed\n");
		exit(-1);
	}
	char *data;
	int filesize;
	filesize = lseek(fd, 0, SEEK_END);
	if((data = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		printf("mmap failed\n");
		exit(-1);
	}
	close(fd);
	regex_t divreg;
	regcomp(&divreg, "<div>\\([^<]\\+\\?\\)</div>", 0);
	regmatch_t match[2];
	char div[1024];
	bzero(div, sizeof(div));
	int index = 0;
	while(regexec(&divreg, data+index, 2, match,0) == 0)
	{
		bzero(div, sizeof(div));
		snprintf(div, match[1].rm_eo - match[1].rm_so + 1, "%s", data + index + match[1].rm_so);
		printf("regex date [%s]\n", div);
		index += match[0].rm_eo;
	}
	regfree(&divreg);
	munmap(data, filesize);
	return 0;
}
