#include <mysocket.h>
#include <pthread.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/epoll.h>

int kil;

//任务类型
typedef struct
{
	void* (*jobs)(void *);//通用函数
	void *arg;//通用参数
}task_t;
//线程池基础属性
typedef struct
{
	int shutdown;//线程池开关
	int max;//线程池最大线程数
	int min;//线程池最小线程数
	int pronumber;//生产者线程个数
	int probusy;//消费者忙碌个数
	int connumber;//消费者线程个数
	int conbusy;//消费者忙碌个数
	int conexitcode;//自杀消费者个数
	int proexitcode;//自杀生产者个数
	int begincon;
	int beginpro;
}pool_t;
//任务对列（循环队列）（用于生产者生产的任务的保存。消费者任务的提取）
typedef struct
{
	int front;//开始位置
	int rear;//结束位置
	int cur;//任务数量
	int max;//队列最大可容数量
	task_t *task;//任务队列
}queue_t;
//条件变量
typedef struct
{
	pthread_cond_t cond_producer;//控制生产者
	pthread_cond_t cond_consumer;//控制消费者
}cond_t;
//锁类型
typedef struct
{
	pthread_mutex_t queuelock;//队列锁
	pthread_mutex_t conbusylock;//消费者忙碌锁
	pthread_mutex_t probusylock;//生产者忙碌锁
	pthread_mutex_t connumberlock;//消费者个数锁
	pthread_mutex_t pronumberlock;//生产者个数锁
	pthread_mutex_t managerlock;//管理者锁
}lock_t;
//线程池里的生产者，消费者，管理者的tid
typedef struct
{
	pthread_t *producers;//生产者tid集合
	pthread_t *consumers;//消费者tid集合
	pthread_t *managers;//管理者tid集合
	pthread_t *sigaction;//捕捉函数tid集合
	pthread_t *alives;
}tid_t;
//线程池属性
typedef struct
{
	pool_t pool;//基础属性
	queue_t queue;//循环队列
	cond_t cond;//条件变量
	lock_t lock;//锁
	tid_t tid;//线程tid集合
	pthread_attr_t *attr;//线程属性
}thread_pool_t;
//线程池
thread_pool_t thread_pool;
int epfd;
int nCount;
struct epoll_event *readyarray;

//网络初始化，返回socket，没做通用处理只能连接tcp形式
int thread_net_init(char *ip, int port);
//创建epoll并且监听文件描述符
int thread_epoll_init(int epollmax);
//线程池初始化（参数线程池基础属性最大最小值，以及缓冲队列的最大值）
int thread_pool_init(int poolmax, int poolmin, int queuemax);
//线程池的销毁
int thread_pool_destroy();
//测试线程是否存活
void* thread_alive_jobs(void *arg);

void* thread_alives(void *arg);
//生产工作
int producer_jobs(task_t *task);
//accept监听
void* thread_socketbusiness(void *arg);
//客户端对接
void* thread_clientbusiness(void *arg);
//信号捕捉
void* thread_sigaction_jobs(void *arg);
//线程池的管理（扩容或者缩减）
void* thread_manger_jobs(void *arg);
//生产者工作
void* thread_producer_jobs(void *arg);
//消费者工作
void* thread_consumer_jobs(void *arg);
