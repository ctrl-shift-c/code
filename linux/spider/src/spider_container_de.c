#include <spider.h>

int spider_container_de(container_t * cont, url_t *url)
{
	if(!cont->size)
	{
		printf("queue size == 0: empty\n");
		return -1;
	}
	
	url->http_type = cont->url_array[cont->front].http_type;
    url->port = cont->url_array[cont->front].port;
    strcpy(url->rurl, cont->url_array[cont->front].rurl);
    strcpy(url->domain, cont->url_array[cont->front].domain);
    strcpy(url->path, cont->url_array[cont->front].path);
    strcpy(url->filename, cont->url_array[cont->front].filename);
    strcpy(url->ip, cont->url_array[cont->front].ip);
	
	cont->rear = (cont->rear+1)%cont->max;
	--cont->size;
	return 0;
}
