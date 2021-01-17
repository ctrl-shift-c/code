#include <spider.h>

int spider_analytical_resource(url_t *url)
{
	int rfd;
	rfd = open(url->filename, O_RDONLY);
	if(-1 == rfd)
	{
		perror("open failed");
		return -1;
	}

	int filesize = lseek(rfd, 0, SEEK_END);
	char filename[2048];
	bzero(filename, sizeof(filename));
	sprintf(filename, "./resfile/%s", url->filename);
	int resfd = open(filename, O_RDWR | O_CREAT, 0664);
	if(-1 == resfd)
	{
		perror("open failed\n");
		return -1;
	}

	char *map;
	map = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, rfd, 0);
	if(MAP_FAILED == map)
	{
		perror("mmap failed");
		return -1;
	}
	close(rfd);
	char *index = map;
	regex_t dreg,hreg;
	regmatch_t dmatch[2];
	regmatch_t hmatch[2];
	char desc[4096];
	char desc_temp[5000];
	char h1[1024];
	char h1_temp[1050];
	bzero(desc, sizeof(desc));
	bzero(h1, sizeof(h1));
	regcomp(&dreg, "<meta name=\"description\" content=\"\\([^\"]\\+\\?\\)\">", 0);
	regcomp(&hreg, "<h1 >\\([^<]\\+\\?\\)</h1>", 0);
	printf("analytical resource successly\n");
	while(regexec(&hreg, index, 2, hmatch, 0) == 0)
	{
		snprintf(h1, hmatch[1].rm_eo - hmatch[1].rm_so + 1, "%s", index + hmatch[1].rm_so);
		sprintf(h1_temp, "<h>%s</h>", h1);
		write(resfd, h1_temp, strlen(h1_temp));
		write(resfd, "\n", 1);
		bzero(h1, sizeof(h1));
		index += hmatch[0].rm_eo;
	}

	index = map;
	while(regexec(&dreg, index, 2, dmatch, 0) == 0)
	{
		snprintf(desc, dmatch[1].rm_eo - dmatch[1].rm_so + 1, "%s", index + dmatch[1].rm_so);
		sprintf(desc_temp, "<p>%s</p>", desc);
		write(resfd, desc_temp, strlen(desc_temp));
		write(resfd, "\n", 1);
		bzero(desc, sizeof(desc));
		index += dmatch[0].rm_eo;
	}
	
	regfree(&dreg);
	regfree(&hreg);
	munmap(map, filesize);
	return 1;
}
