#include <spider.h>

int main(int args, char **argv)
{
	/*
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

	ssl_t *ssl = NULL;
	printf("%d\n", url.http_type);
	if(url.http_type == 1)
		ssl = spider_openssl_create(webfd);
	spider_response_download(webfd, reqhead, &url, ssl);
	printf("[6] download success\n");
	*/
	url_t url;
	container_t *contain_w;
	container_t *contain_y;
	contain_w = spider_container_create(200);
	contain_y = spider_container_create(200);
	strcpy(url.filename, "100571?fr=aladdin");
//	spider_analytical_moreurl(&url, contain_w, contain_y);
	spider_analytical_resource(&url);
//	close(webfd);
	return 0;
}
