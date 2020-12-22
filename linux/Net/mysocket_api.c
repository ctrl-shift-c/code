#include "./mysocket_api.h"

int BUSINESS_01(int sockfd)
{
	char buffer[1500];
	int recvlen;
	int j;
	bzero(buffer, sizeof(buffer));
	while((recvlen = recv(sockfd, buffer, sizeof(buffer), 0))>0)
	{
		j=0;
		while(recvlen >j)
		{
				buffer[j] = toupper(buffer[j]);
				j++;
		}
		if(send(sockfd, buffer, recvlen, 0) == -1)
		{
				STRERROR("send response msg error", 0);
				return -1;
		}
		bzero(buffer, sizeof(buffer));
	}
	//检测客户端异常退出
	if(recvlen == 0)
	{
		close(sockfd);
		printf("client exiting .. child processe pid %d exit.\n", getpid());
		exit(0);
	}
	else if(recvlen == -1)
	{
		STRERROR("recv request msg error", 0);
		close(sockfd);
		return -1;
	}
	return 0;
}

int SOCKET(int domain , int type , int protocol)
{
		int reval;
		if((reval = socket(domain,type,protocol))==-1)
				perror("SOCKET CALL FAILED");
		return reval;
}

int BIND(int sockfd, const struct sockaddr *addr,socklen_t addrlen)
{
		int reval;
		if((reval = bind(sockfd,addr,addrlen))==-1)
				perror("BIND CALL FAILED");
		return reval;
}

int LISTEN(int sockfd, int backlog)
{
		int reval;
		if((reval = listen(sockfd,backlog))==-1)
				perror("LISTEN CALL FAILED");
		return reval;
}

int ACCEPT(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
		int reval;
		if((reval = accept(sockfd,addr,addrlen))==-1)
				perror("ACCEPT CALL FAILED");
		return reval;
}

int CONNECT(int sockfd, const struct sockaddr *addr,socklen_t addrlen)
{
		int reval;
		if((reval = connect(sockfd,addr,addrlen))==-1)
				perror("CONNECT CALL FAILED");
		return reval;
}

ssize_t RECV(int sockfd, void *buf, size_t len, int flags)
{
		ssize_t reval;
		if((reval = recv(sockfd,buf,len,flags))==-1)
				perror("RECV CALL FAILED");
		return reval;
}

ssize_t SEND(int sockfd, void *buf, size_t len, int flags)
{
		ssize_t reval;
		if((reval = send(sockfd,buf,len,flags))==-1)
				perror("SEND CALL FAILED");
		return reval;
}

ssize_t SENDTO(int sockfd, const void *buf, size_t len, int flags,const struct sockaddr *dest_addr, socklen_t addrlen)
{
		int reval;
		if((reval = sendto(sockfd,buf,len,flags,dest_addr,addrlen))==-1)
				perror("SENDTO CALL FAILED");
		return reval;
}

ssize_t RECVFROM(int sockfd, void *buf, size_t len, int flags,struct sockaddr *src_addr, socklen_t *addrlen)
{
		int reval;
		if((reval = recvfrom(sockfd,buf,len,flags,src_addr,addrlen))==-1)
				perror("RECVFROM CALL FAILED");
		return reval;
}

int NET_INIT(int flags,int type , const char * ip , int port) // flags(初始化应用端 0 ser , 1 client)  type (0 tcp 1 udp)
{
		struct sockaddr_in serveraddr,clientaddr;
		int sockfd;
		if(flags==0 && type==0){
				bzero(&serveraddr,sizeof(serveraddr));
				serveraddr.sin_family = AF_INET	;
				serveraddr.sin_port = htons(port);
				inet_pton(AF_INET,ip,&serveraddr.sin_addr.s_addr);
				sockfd = SOCKET(AF_INET,SOCK_STREAM,0);
				BIND(sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
				LISTEN(sockfd,128);
		}
		else if(flags==0 && flags==1)
		{
				bzero(&serveraddr,sizeof(serveraddr));
				serveraddr.sin_family = AF_INET	;
				serveraddr.sin_port = htons(port);
				inet_pton(AF_INET,ip,&serveraddr.sin_addr.s_addr);
				sockfd = SOCKET(AF_INET,SOCK_DGRAM,0);
				BIND(sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
		}
		else if(flags==1 && type==0)
				sockfd = SOCKET(AF_INET,SOCK_STREAM,0);
		else if(flags==1 && type==1)
				sockfd = SOCKET(AF_INET,SOCK_DGRAM,0);
		return sockfd;
}

//再次对RECV or SEND进行包裹
//读写一个字符
//读写一行
//读写指定的数据量

void STRERROR(const char *errstr, int errcode)
{
	if(errcode)
		printf("%s:%s", errstr, strerror(errcode));
	else
		perror(errstr);
}
