#include <spider.h>

int spider_analytical_moreurl(url_t *url, container_t *wcont, container_t *ycont)
{
	int rfd;
	rfd = open(url->filename, O_RDONLY);
	if(-1 == rfd)
	{
		perror("opne filename failed\n");
		return -1;
	}
	char *map;
	char *index;
	int filesize = lseek(rfd, 0, SEEK_END);
	map = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE,rfd, 0);
	index = map;
	if(MAP_FAILED == mmap)
	{
		perror("mmap failed\n");
		return -1;
	}
	close(rfd);
	regmatch_t amatch[3];
	regex_t areg;
	regcomp(&areg, "<a[^>]*\\?href=\"\\(/item/[^\"]*\\?\\)\"[^>]\\+\\?>\\([^<]*\\?\\)</a>", 0);
	char a_rurl[8192];
	char a_title[1024];
	bzero(a_rurl, sizeof(a_rurl));
	bzero(a_title, sizeof(a_title));
	while(regexec(&areg, index, 3, amatch, 0) == 0)
	{
		snprintf(a_rurl, amatch[1].rm_eo - amatch[1].rm_so + 24, "https://baike.baidu.com%s", index+amatch[1].rm_so);
		snprintf(a_title, amatch[2].rm_eo - amatch[2].rm_so + 1, "%s", index+amatch[2].rm_so);
		index += amatch[0].rm_eo;
//		printf("url = [%s] title = [%s]\n",a_rurl, a_title);
		if(spider_container_removeDup(wcont,ycont,a_rurl))
		{
			spider_container_en(wcont, a_rurl);
			printf("add url = [%s]\n",a_rurl);
		}
		bzero(a_rurl, sizeof(a_rurl));
		bzero(a_title, sizeof(a_title));
	}
	regfree(&areg);
	munmap(mmap, filesize);
	return 0;
}
