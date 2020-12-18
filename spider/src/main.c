#include <spider.h>

int main(int args, char **argv)
{
	if(args != 2 || strlen(argv[1])<=0)
	{
		printf("main args numner..\n");
		exit(-1);
	}
	url_t url;
	strcpy(url.rurl, argv[1]);
	printf("[1] url access successly...\n");
	if( spider_url_analytical(&url) == -1 )
	{
		printf("url not right..\n");
		exit(-1);
	}
	printf("[2] url analytical successly...\n");
	int webfd;
	webfd = NET_INIT(CLIENT_INIT, PROTOCOL_TCP, NULL, 0);
	if(webfd == -1)
		exit(-1);
	spider_connect_wait(webfd, &url);
	printf("[3] client connect successly...\n");
	char reqhead[4096];
	spider_create_requesthead(reqhead, &url);
	printf("[4] requesthead create successly...\n");
	spider_response_download(webfd, reqhead, &url);
	printf("[6] download success\n");
	close(webfd);
	return 0;
}
