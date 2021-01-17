#include <spider.h>

container_t *spider_container_create(int queuemax)
{
	container_t *cont = NULL;
	cont = (container_t *)malloc(sizeof(container_t));
	if(cont == NULL)
	{
		perror("malloc container_t failed");
		return NULL;
	}
	bzero(cont, sizeof(container_t));
	cont->url_array = (url_t *)malloc(sizeof(url_t)*queuemax);
	if(cont->url_array == NULL)
	{
		perror("malloc url_array failed");
		return NULL;
	}
	bzero(cont, sizeof(cont->url_array));
	cont->max = queuemax;
	return cont;
}
