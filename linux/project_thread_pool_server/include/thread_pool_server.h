#include <pthread.h>
#include <signal.h>
#include <sys/epoll.h>
#include <errno.h>

#define _EPOLL_MAXSIZE_ 200000
int epfd;//全局监听树
pthread_mutex_t acclock;
pthread_mutex_t epolllock;

typedef struct
{
	void* (*jobs)(void *);
	void *arg;
}task_t;


typedef struct
{
	int thread_shutdown;
	int thread_max;
	int thread_min;
	int thread_alive;
	int thread_busy;
	
	int thread_exitcode;
	
	task_t *queue_task;
	int queue_front;
	int queue_rear;
	int queue_cur;
	int queue_max;
	
	pthread_t *customer_id;
	pthread_t *producer_id;
	pthread_t manager_id;

	pthread_mutex_t queuelock;
	pthread_cond_t not_full;
	pthread_cond_t not_empty;
}thread_pool_t;

thread_pool_t* thread_pool_create(int, int, int);
int thread_pool_destroy(thread_pool_t *);
int thread_epoll_create(int, int);
int thread_producer_jobs(thread_pool_t *, task_t);
void *thread_customer_jobs(void *);
void *thread_manager_jobs(void *);
void *BUSINESS_A(void *);
void *BUSINESS_B(void *);
int thread_test_alive(pthread_t tid);
