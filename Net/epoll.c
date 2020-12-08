#include <mysocket_api.h>
#include <sys/epoll.h>//epoll头文件

#define SERVER_SHUTDOWN 1
int main(void)
{
	int serverfd;
	struct sockaddr_in clientaddr;
	socklen_t addrlen;
	int readycode;
	int clientfd;
	int recvlen,j,i;
	char buffer[1500];
	char ip[16];
	bzero(buffer, sizeof(buffer));
	bzero(ip, sizeof(ip));
	struct epoll_event ready_array[200000];//就绪数组，用于epoll传出就绪的节点，用于用户遍历处理
	//创建epoll监听树
	//建议创建判断错误
	int epfd = epoll_create(200000);
	struct epoll_event tmp;//临时节点类型, 用户添加修改节点
	//网络初始化
	serverfd = NET_INIT(SERVER_INIT, PROTOCOL_TCP, "192.168.102.100", 8000);
	//初始化第一个监听的节点
	tmp.data.fd = serverfd;
	tmp.events = EPOLLIN;
	//将节点添加到监听树中
	epoll_ctl(epfd, EPOLL_CTL_ADD, serverfd, &tmp);
	printf("epoll server version 1.0 runing...\n");
	while(SERVER_SHUTDOWN)
	{
		readycode = epoll_wait(epfd, ready_array, 200000, -1);
		i = 0;
		while(readycode)
		{
			addrlen = sizeof(clientaddr);
			if(ready_array[i].data.fd == serverfd)
			{
				//与客户端建立连接，并将客户端的sock添加到监听集合中
				clientfd = ACCEPT(ready_array[i].data.fd, (struct sockaddr *)&clientaddr, &addrlen);
				tmp.data.fd = clientfd;
				tmp.events = EPOLLIN;
				epoll_ctl(epfd, EPOLL_CTL_ADD, clientfd, &tmp);
			}
			else
			{
				//为客户处理一次请求，如果客户端异常，将该客户端sock的监听移除
				if((recvlen = RECV(ready_array[i].data.fd, buffer, sizeof(buffer), 0))>0)
				{
					j = 0;
					while(recvlen > j)
					{
						buffer[j] = toupper(buffer[j]);
						j++;
					}
					SEND(ready_array[i].data.fd, buffer, recvlen, 0);
					bzero(buffer, sizeof(buffer));
				}
				
				if(recvlen == 0)
				{
					epoll_ctl(epfd, EPOLL_CTL_DEL, ready_array[i].data.fd, NULL);
					close(ready_array[i].data.fd);
					printf("client quit..\n");
				}
			}
			i++;
			readycode--;
		}
	}
}
