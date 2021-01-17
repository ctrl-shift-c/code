#include <mysocket.h>

int SOCKET(int domain, int type, int protocol)
{
	int sockfd = socket(domain, type, protocol);
	SYSTEM_ERROR(sockfd != -1, "socket failed", SHUTUP);
	return sockfd;
}

void SETSOCKOPT(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
{
	int status = setsockopt(sockfd, level, optname, optval, optlen);
	SYSTEM_ERROR(status != -1, "setsockopt failed", SHUTUP);
	return;
}

void BIND(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int bin = bind(sockfd, addr, addrlen);
	SYSTEM_ERROR(bin != -1, "bind failed", SHUTUP);
	return;
}

void LISTEN(int sockfd, int backlog)
{
	int lis = listen(sockfd, backlog);
	SYSTEM_ERROR(lis != -1, "listen failed", SHUTUP);
	return;
}

int ACCEPT(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
	int serclientfd = accept(sockfd, addr, addrlen);
	SYSTEM_ERROR(serclientfd != -1, "accept failed", SHUTUP);
	return serclientfd;
}

ssize_t SEND(int sockfd, const void *buf, size_t len, int flags)
{
	int sen = send(sockfd, buf, len, flags);
	USER_ERROR(sen != -1, "client quit", SHUTDOWN);
	return sen;
}

ssize_t RECV(int sockfd, void *buf, size_t len, int flags)
{
	int rec = recv(sockfd, buf, len, flags);
	USER_ERROR(rec != -1, "client quit", SHUTDOWN);
	return rec;
}
void CONNECT(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int con = connect(sockfd, addr, addrlen);
	SYSTEM_ERROR(con !=-1, "connect failed", SHUTUP);
	return;
}
void SYSTEM_ERROR(int flags, char *err, int shutdown)
{
	if(!flags)
	{
		if(errno == EINTR)
			return;
		if(errno == EAGAIN)
			return;
		perror(err);
		if(shutdown)
		{
			exit(-1);
		}
	}
}
void USER_ERROR(int flags, char *err, int shutdown)
{
	if(!flags)
	{
		printf("%s\n",err);
		if(shutdown)
		{
			exit(-1);
		}
	}
}

