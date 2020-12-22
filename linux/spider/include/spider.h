#include <mysocket_api.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <regex.h>

typedef struct 
{
	int http_type;
	int port;
	char rurl[4096];
	char domain[1024];
	char path[1024];
	char filename[1024];
	char ip[16];
}url_t;

int spider_url_analytical(url_t *);//解析url
int spider_connect_wait(int webfd, url_t *url);//客户端连接
int spider_create_requesthead(char *reqhead, url_t *url);
int spider_response_download(int webfd, char *reqhead, url_t *url);
int spider_get_statuscode(char *reshead);
