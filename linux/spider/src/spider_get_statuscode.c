#include <spider.h>

int spider_get_statuscode(char *reshead)
{
	/*
	char *pos = strstr(reshead, "HTTP/1.1");
	char codebuf[4];
	strncpy(codebuf, pos+9, 3);
	printf("codebuf = %s\n",codebuf);
	return atoi(codebuf);
	*/
	int status = 0;
	regex_t reg;
	regcomp(&reg, "HTTP/1.1 \\(...\\)", 0);
	regmatch_t match[2];
	char csta[4];
	if(regexec(&reg, reshead, 2, match, 0) == 0)
	{
		csta[0] = reshead[match[1].rm_so];
		csta[1] = reshead[match[1].rm_so+1];
		csta[2] = reshead[match[1].rm_so+2];
		csta[3] = '\0';
		printf("status %s\n", csta);
	}
	else
		printf("reg failed\n");
	status = atoi(csta);
	return status;
}
