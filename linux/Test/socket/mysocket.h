#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>

#define NET_INIT_TCP_SERVER 0
#define NET_INIT_TCP_CLIENT 1

int SOCKET(int domain, int type, int protocol);
void SETSOCKOPT(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
void BIND(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
void LISTEN(int sockfd, int backlog);
int ACCEPT(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
ssize_t SEND(int sockfd, const void *buf, size_t len, int flags);
ssize_t RECV(int sockfd, void *buf, size_t len, int flags);
void CONNECT(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
void SYSTEM_ERROR(int flags, char *err);
