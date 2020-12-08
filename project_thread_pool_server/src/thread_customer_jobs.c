#include <thread_pool_server.h>
#include <mysocket_api.h>
void *thread_customer_jobs(void *arg)
{
	thread_pool_t *ptr = (thread_pool_t *)arg;
	task_t temp;
	printf("customer thread 0x%x waiting jobs..\n", (unsigned int)pthread_self());
	while(ptr->thread_shutdown)
	{
		printf("customer_1 busy %d exit %d\n", ptr->thread_busy, ptr->thread_exitcode);
		pthread_mutex_lock(&ptr->queuelock);
		while(!(ptr->queue_cur) && ptr->thread_exitcode ==0)
			pthread_cond_wait(&(ptr->not_empty), &(ptr->queuelock));
		if(ptr->thread_exitcode > 0)
		{
			--ptr->thread_alive;
			--ptr->thread_exitcode;
			pthread_mutex_unlock(&ptr->queuelock);
			printf("manager kill customer thread 0x%x exiting..\n",(unsigned int)pthread_self());
			pthread_exit(NULL);
		}
		if(!(ptr->thread_shutdown))
		{
			--ptr->thread_alive;
			pthread_mutex_unlock(&ptr->queuelock);
			printf("thread pool close, customer thread 0x%x exiting..\n", (unsigned int)pthread_self());
			pthread_exit(NULL);
		}
		printf("customer_2 busy %d exit %d\n", ptr->thread_busy, ptr->thread_exitcode);
		temp.jobs = ptr->queue_task[ptr->queue_rear].jobs;
		temp.arg = ptr->queue_task[ptr->queue_rear].arg;
		--ptr->queue_cur;
		ptr->queue_rear = (ptr->queue_rear + 1) % ptr->queue_max;
		++ptr->thread_busy;
		pthread_cond_signal(&ptr->not_full);
		pthread_mutex_unlock(&ptr->queuelock);
		
		if(BUSINESS_A == temp.jobs)
			printf("customer BUSINESS_A ..\n");
		else
			printf("customer BUSINESS_B ..\n");
		temp.jobs(temp.arg);
		printf("customer_3 busy %d exit %d\n", ptr->thread_busy, ptr->thread_exitcode);
		pthread_mutex_lock(&ptr->queuelock);
		--ptr->thread_busy;
		pthread_mutex_unlock(&ptr->queuelock);
		printf("thread busy %d\n", ptr->thread_busy);
	}
	pthread_exit(NULL);
}
