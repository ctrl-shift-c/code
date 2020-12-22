/* 多进程服务端模型， 可以完成1对多的连接与业务处理 */
#include <mysocket_api.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#define SERVER_SHUTDOWN 1

void sig_wait(int n)
{
	printf("sig_wait\n");
	pid_t wpid;
	int status;
	//WNOHANG 非阻塞
	while((wpid = waitpid(-1, &status, WNOHANG))>0)
	{
		//通过宏函数判断处理子进程的退出原因
		if(WIFEXITED(status))
			printf("wait_thread [0x%x] wait success, zombie pro [%d] , client process exit code [%d]\n", 
						(unsigned int)pthread_self(), wpid, WEXITSTATUS(status));
		if(WIFSIGNALED(status))
             printf("wait_thread [0x%x] wait success, zombie pro [%d] , kill child signal [%d]\n",
                       	(unsigned int)pthread_self(), wpid, WTERMSIG(status));
	}
}

void *thread_wait(void *arg)
{
	printf("thread_wait\n");
	//分离态
	pthread_detach(pthread_self());
	//信号捕捉(SIGCHLD)
	//定义信号结构体
	struct sigaction act, oact;
	//选择行为
	//捕捉传入函数地址
	//SIG_DEF默认
	//SIG_IGN忽略
	act.sa_handler = sig_wait;
	act.sa_flags = 0;
	//将全部信号置0
	sigemptyset(&act.sa_mask);
	//替换行为，将自定义信号行为设置为默认行为
	sigaction(SIGCHLD, &act, &oact);
	//解除屏蔽
	sigprocmask(SIG_SETMASK, &act.sa_mask, NULL);
	//等待信号
	while(1)
		sleep(1);
	pthread_exit(NULL);
	
}


int main(void)
{
	struct sockaddr_in clientaddr;
	int serverfd, clientfd;
	socklen_t addrsize;
	serverfd = NET_INIT(0, 0, "192.168.102.100", 8000);
	char ip[16];
	char buffer[1500];
	bzero(buffer, sizeof(buffer));
	bzero(ip, sizeof(ip));
	int recvlen;
	int j;
	int err;
	pthread_t tid;
	pid_t pid;
	//设置对SIGCHLD信号的屏蔽（继承给普通线程）
	// 定义屏蔽子
	sigset_t set, oset; 
	//将所有信号置0
	sigemptyset(&set);
	//将指定位置信号置1
	sigaddset(&set, SIGCHLD);
	//替换信号 
	sigprocmask(SIG_SETMASK, &set, &oset);
	
	if((err == pthread_create(&tid, NULL, thread_wait, NULL))>0)
		STRERROR("thread create call failed", err);
	
	while(SERVER_SHUTDOWN)
	{
		//要记得每次都更改，初始化，该值可能被修改
		addrsize = sizeof(clientaddr);
		if((clientfd = ACCEPT(serverfd,(struct sockaddr *) &clientaddr, &addrsize))>0)
		{
			printf("server parent process %d client ip %s port %d\n",
				getpid(), inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ip, sizeof(ip)), ntohs(clientaddr.sin_port));
			pid = fork();
			if(pid > 0)
			{
				printf("server parent process pid %d create pro successly..\n",getpid());
			}
			else if(pid == 0)
			{
				while((recvlen = recv(clientfd, buffer, sizeof(buffer),0))>0)
				{
					j = 0;
					while(recvlen > j)
					{
						buffer[j] = toupper(buffer[j]);
						j++;
					}
					if(send(clientfd, buffer, recvlen, 0)==-1)
						STRERROR("send response msg error", 0);
					bzero(buffer, sizeof(buffer));
				}
				//检测客户端异常退出
				if(0 == recvlen)
				{
					printf("client exiting... client process pid %d exit.\n", getpid());
					exit(0);
				}
			}
			else
			{
				STRERROR("fork call failed", 0);
			}
		}
		
	}
	close(serverfd);
	return 0;
}
