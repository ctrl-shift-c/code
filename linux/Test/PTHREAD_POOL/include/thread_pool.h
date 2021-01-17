#include <mysocket.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <signal.h>

//任务类型，模仿的pthread_create的执行方式
typedef struct
{
	void* (*jobs)(void *arg);
	void *arg;
}task_t;

typedef struct
{
	int shutdown;
    int max;
    int min;
    int busy;
    int alive;
	int exitcode;
	pthread_attr_t *attr;
}pool_t;

typedef struct
{
    int front;
    int rear;
    int cur;
    int max;
    task_t *task;
}queue_t;

typedef struct
{
    pthread_cond_t producers;
    pthread_cond_t consumers;
}cont_t;

typedef struct
{
	pthread_t *consumer_id;
    pthread_t *producer_id;
    pthread_t *manager_id;
	pthread_t *sigset_id;
}thread_id_t;

typedef struct
{
	pthread_mutex_t queuelock;
	pthread_mutex_t acceptlock;
	pthread_mutex_t epolllock;
}mutex_t;

typedef struct
{
	pool_t pool;
	queue_t queue;
	thread_id_t id;
	cont_t cont;
	mutex_t mutex;
}thread_pool_t;

thread_pool_t thread_pool;
int epfd;

int net_init(char *ip, int port);
int thread_epoll_create(int max, int socket);
int thread_pool_init(int max, int min, int queuemax);
int thread_pool_destroy();
int thread_producer_jobs(task_t *jobs);
void *thread_consumer_jobs(void *arg);
void *thread_pool_manager_jobs(void *arg);
void *BUSINESS_ACCEPT(void *arg);
void *BUSINESS_SEND(void *arg);
void *thread_pool_sigset_jobs(void *arg);
int thread_test_alive(pthread_t tid);


