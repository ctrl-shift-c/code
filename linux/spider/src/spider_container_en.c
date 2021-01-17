#include <spider.h>

int spider_container_en(container_t * cont, const char *rurl)
{
	if(cont->size == cont->max)
	{
		printf("en failed: queue full\n");
		return -1;
	}
	strcpy(cont->url_array[cont->front].rurl, rurl);
	/*
	cont->url_array[cont->front].http_type = url->http_type;
	cont->url_array[cont->front].port = url->port;
	strcpy(cont->url_array[cont->front].rurl, url->rurl);
	strcpy(cont->url_array[cont->front].domain, url->domain);
	strcpy(cont->url_array[cont->front].path, url->path);
	strcpy(cont->url_array[cont->front].filename, url->filename);
	strcpy(cont->url_array[cont->front].ip, url->ip);	
	*/
	cont->front = (cont->front + 1) % cont->max;
	++cont->size;
	return  0;
}
