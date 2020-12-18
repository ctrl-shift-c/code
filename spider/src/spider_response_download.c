#include <spider.h>

int spider_response_download(int webfd, char *reqhead, url_t *url)
{
	char reshead[4096];
	char recvbuf[8192];
	char *pos = NULL;
	bzero(reshead, sizeof(reshead));
	bzero(recvbuf, sizeof(recvbuf));
	SEND(webfd, reqhead, strlen(reqhead), 0);
	int recvlen; 
	if( (recvlen = RECV(webfd, recvbuf, sizeof(recvbuf), 0)) >0 )
	{
		if( (pos = strstr(recvbuf, "\r\n\r\n")) == NULL )
		{
			printf("response access failed...\n");
			exit(-1);
		}
		snprintf(reshead, pos-recvbuf+4, "%s", recvbuf);
		printf("reshead get success:\n%s\n", reshead);
		int fd;
		if( (fd = open(url->filename, O_CREAT|O_RDWR, 0775)) <0 )
		{
			printf("create net file failed\n");
			exit(-1);
		}
		write(fd, pos+4, recvlen - (pos - recvbuf + 4));
		bzero(recvbuf, sizeof(recvbuf));
		while( (recvlen = RECV(webfd, recvbuf, sizeof(recvbuf), 0)) >0 )
		{
			write(fd, recvbuf, recvlen);
		}
		close(fd);
	}
	return 0;
}
