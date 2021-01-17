#include <spider.h>

int spider_container_removeDup(container_t *wcont, container_t *ycont, const char *url)
{
	int flags = wcont->rear;
	while(flags % wcont->max != wcont->front)
	{
		if(strncmp(wcont->url_array[flags].rurl, url, strlen(url)) == 0)
		{
			return 0;
		}
		flags++;
	}
	flags = ycont->rear;
	while(flags % ycont->max != ycont->front)
    {
        if(strncmp(ycont->url_array[flags].rurl, url, strlen(url)) == 0)
        {
            return 0;
        }
        flags++;
	}
	return 1;
}
