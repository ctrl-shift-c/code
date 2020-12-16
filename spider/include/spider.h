#include <mysocket_api.h>

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

int spider_url_analytical(url_t *);

