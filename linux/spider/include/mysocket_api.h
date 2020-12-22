#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SERVER_INIT 0
#define CLIENT_INIT 1
#define PROTOCOL_TCP 0
#define PROTOCOL_UDP 1

int BUSINESS_01(int sockfd);

int SOCKET(int domain , int type , int protocol);

int BIND(int sockfd, const struct sockaddr *addr,socklen_t addrlen);

int LISTEN(int sockfd, int backlog);

int ACCEPT(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

int CONNECT(int sockfd, const struct sockaddr *addr,socklen_t addrlen);

ssize_t RECV(int sockfd, void *buf, size_t len, int flags);

ssize_t SEND(int sockfd, void *buf, size_t len, int flags);

ssize_t SENDTO(int sockfd, const void *buf, size_t len, int flags,const struct sockaddr *dest_addr, socklen_t addrlen);

ssize_t RECVFROM(int sockfd, void *buf, size_t len, int flags,struct sockaddr *src_addr, socklen_t *addrlen);

int NET_INIT(int flags,int type , const char * ip , int port);


// 第一个参数是用户自定义的报错信息，第二个为标志0系统报错， 1线程报错方式
void STRERROR(const char *errstr, int errcode);
/*线程错误处理
if((int err = pthread_create)>0)
strerror(error);
//系统函数错误处理
if((open()==-1)
perror("错误信息");
*/
