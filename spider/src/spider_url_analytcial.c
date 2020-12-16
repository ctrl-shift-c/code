#include <spider.h>

int spider_url_analytical(url_t *u)
{
	if(u == NULL && strlen(u->rurl)<=0)return -1;

	bzero(u->domain, sizeof(u->domain));
	bzero(u->path, sizeof(u->path));
	bzero(u->filename, sizeof(u->filename));
	bzero(u->ip, sizeof(u->ip));

	int total_len = 0;
	int path_len = 0;
	int domain_len = 0;
	int filename_len = 0;

	const char *array[]={"http://", "https://"};
	if(strncmp(u->rurl, array[0], strlen(array[0])) == 0)
	{
		u->http_type = 0;
		u->port = 80;
		total_len = strlen(array[0]);

	}
	else
	{
		u->http_type = 1;
		u->port = 443;
		total_len = strlen(array[1]);
	}

	char ctemp[1024];
	bzero(ctemp, sizeof(ctemp));
	domain_len = 0;
	while(u->rurl[total_len] != '/' && u->rurl[total_len] != '\0')
	{
		ctemp[domain_len++] = u->rurl[total_len++];
	}

	ctemp[domain_len] = '\0';

	strcpy(u->domain, ctemp);
	bzero(ctemp, sizeof(ctemp));
	total_len++;
	path_len = 0;
	filename_len = 0;
	while(u->rurl[total_len] != '\0')
	{
		filename_len++;
		if(u->rurl[total_len] == '/')
			filename_len = 0;
		ctemp[path_len++] = u->rurl[total_len++];
	}
	path_len = path_len - filename_len;
	strncpy(u->path, ctemp, path_len);
	u->path[path_len-1] = '\0';
	for(int i=path_len,j=0; i<total_len; i++, j++)
	{
		u->filename[j] = ctemp[i];
	}
	u->filename[filename_len] = '\0';
	struct hostent  *ent = NULL;
	if( (ent = gethostbyname(u->domain))==NULL  )
	{
		perror("gethostname call failed...");
		return -1;
	}
	inet_ntop(AF_INET, ent->h_addr_list[0], u->ip, 16);
	printf("%s %s %s %s\n", u->domain, u->path, u->filename, u->ip);
	return 1;
}
