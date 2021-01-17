#include <thread_pool.h>
#define PORT 8000
#define IP "192.168.102.100"

void* thread_producer_jobs(void *arg)
{
	int serverfd = thread_net_init(IP, PORT);
	printf("thread_net_init successly\n");

	struct sockaddr_in clientaddr;
	socklen_t clientlen;
	int serclientfd;

	fd_set nset, oset;
	FD_SET(serverfd, &nset);
	int maxfd = serverfd;
	struct timeval mytime;
	int readycode ;

	while(thread_pool.pool.shutdown)
	{
		readycode = 0;
		while(0 == readycode)
		{
			oset = nset;
			mytime.tv_sec = 10;
			mytime.tv_usec = 0;
			readycode = select(maxfd+1, &oset, NULL, NULL, &mytime);
			/*
			if(readycode == 0 && thread_pool.pool.proexitcode > 0)
			{
//				printf("manager begin kill producer\n");
				close(serverfd);
				pthread_mutex_lock(&thread_pool.lock.pronumberlock);
				--thread_pool.pool.proexitcode;
				--thread_pool.pool.pronumber;
				pthread_mutex_unlock(&thread_pool.lock.pronumberlock);
//				pthread_kill(thread_pool.tid.sigaction[0], 34);
				pthread_exit(NULL);
			}
			*/
		}
		clientlen = sizeof(clientaddr);
		serclientfd = ACCEPT(serverfd, (struct sockaddr *)&clientaddr, &clientlen);
		printf("%d\n", nCount++);
		int flags=0;
		SYSTEM_ERROR((flags = fcntl(serclientfd, F_GETFL, 0)) != -1, "sockbusiness fcntl get failed", SHUTUP);
		SYSTEM_ERROR(fcntl(serclientfd, F_SETFL, flags | O_NONBLOCK) != -1, "sockbusiness fcntl set failed", SHUTUP);
		struct epoll_event temp;
		temp.data.fd = serclientfd;
		temp.events = EPOLLIN | EPOLLET;
		SYSTEM_ERROR(epoll_ctl(epfd, EPOLL_CTL_ADD, serclientfd, &temp) == 0, "epoll_crl add serclientfd failed", SHUTUP);
		char ip[16];
		inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, 16);
		printf("port = [%d]\nip=[%s]\n", ntohs(clientaddr.sin_port), ip);
	}
	return NULL;
}
