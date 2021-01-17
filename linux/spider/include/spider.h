#include <mysocket_api.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <regex.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

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

typedef struct
{
	SSL * sslsocket;
	SSL_CTX *sslctx;
}ssl_t;

typedef struct
{
	int front;
	int rear;
	int size;
	int max;
	url_t *url_array;
}container_t;

int spider_url_analytical(url_t *);//解析url
int spider_analytical_resource(url_t *);
int spider_analytical_moreurl(url_t *, container_t *, container_t *);

int spider_connect_wait(int webfd, url_t *url);//客户端连接
int spider_create_requesthead(char *reqhead, url_t *url);
int spider_response_download(int webfd, char *reqhead, url_t *url, ssl_t *ssl);
int spider_get_statuscode(char *reshead);
ssl_t *spider_openssl_create(int webfd);

container_t *spider_container_create(int);
int spider_container_en(container_t *, const char *);
int spider_container_de(container_t *, url_t *);
int spider_container_removeDup(container_t *, container_t *, const char *);


