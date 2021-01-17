#include "mysocket.h"

int SOCKET(int domain, int type, int protocol)
{
	int sockfd = socket(domain, type, protocol);
	SYSTEM_ERROR(sockfd, "socket failed");
	return sockfd;
}

void SETSOCKOPT(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
{
	int status = setsockopt(sockfd, level, optname, optval, optlen);
	SYSTEM_ERROR(status, "setsockopt failed");
}

void BIND(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int bin = bind(sockfd, addr, addrlen);
	SYSTEM_ERROR(bin, "bind failed");
}

void LISTEN(int sockfd, int backlog)
{
	int lis = listen(sockfd, backlog);
	SYSTEM_ERROR(lis, "listen failed");
}

int ACCEPT(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
	int serclientfd = accept(sockfd, addr, addrlen);
	SYSTEM_ERROR(serclientfd, "accept failed");
	return serclientfd;
}

ssize_t SEND(int sockfd, const void *buf, size_t len, int flags)
{
	int sen = send(sockfd, buf, len, flags);
	return sen;
}

ssize_t RECV(int sockfd, void *buf, size_t len, int flags)
{
	int rec = recv(sockfd, buf, len, flags);
	SYSTEM_ERROR(rec, "client quit");
	return rec;
}
void CONNECT(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int con = connect(sockfd, addr, addrlen);
	SYSTEM_ERROR(con, "connect failed");
}

void SYSTEM_ERROR(int flags, char *err)
{
	if(-1 == flags)
	{
		if(errno == EINTR)
			return;
		perror(err);
		exit(-1);
	}
}

void USER_ERROR(int flags, char *err)
{
    if(-1 == flags)
    {
        printf("%s\n",err);
        exit(-1);
    }
}


