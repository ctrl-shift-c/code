#include <spider.h>

int main(int args, char **argv)
{
	if(args != 2 || strlen(argv[1])<=0)
	{
		printf("main args numner..\n");
	}
	url_t url;
	strcpy(url.rurl, argv[1]);
	if( spider_url_analytical(&url) == -1 )
	{
		printf("url not right..\n");
	}
}
