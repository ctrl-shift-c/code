#include <spider.h>

int spider_connect_wait(int webfd, url_t *url)
{
	struct sockaddr_in webaddr;
	bzero(&webaddr, sizeof(webaddr));
	webaddr.sin_family = AF_INET;
	webaddr.sin_port = htons(url->port);
	inet_pton(AF_INET, url->ip, &webaddr.sin_addr.s_addr);
	return CONNECT(webfd, (struct sockaddr *)&webaddr, sizeof(webaddr));
}
