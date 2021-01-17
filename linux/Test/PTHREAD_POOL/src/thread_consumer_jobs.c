#include <thread_pool.h>

void *thread_consumer_jobs(void *arg)
{
//	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
//	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);  
	while(thread_pool.pool.shutdown)
	{
//		pthread_testcancel();
//		pthread_cleanup_push(pthread_mutex_unlock, (void *)&thread_pool.mutex.queuelock);
		pthread_mutex_lock(&thread_pool.mutex.queuelock);
//		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
//      pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
//		printf("1 consumer lock\n");
		
		while(!thread_pool.queue.cur)
		{
//			printf("1 consumer unlock\n");
//			pthread_cleanup_push(pthread_mutex_unlock, (void *)&thread_pool.mutex.queuelock);
			pthread_cond_wait(&thread_pool.cont.consumers, &thread_pool.mutex.queuelock);
//			pthread_cleanup_pop(0);

//     		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
//	        pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

//			pthread_mutex_lock(&thread_pool.mutex.queuelock);
 //           ++thread_pool.pool.alive;
  //          pthread_mutex_unlock(&thread_pool.mutex.queuelock);
//			printf("2 consumer lock\n");
			if(thread_pool.pool.exitcode > 0)
			{
				--thread_pool.pool.alive;
				--thread_pool.pool.exitcode;
				pthread_mutex_unlock(&thread_pool.mutex.queuelock);
				printf("manger kill customer thread 0x%x exiting\n", (unsigned int)pthread_self());
//				printf("alive %d\nbusy %d\n", thread_pool.pool.alive, thread_pool.pool.busy);
				pthread_exit(NULL);
			}
		}
		if(!thread_pool.pool.shutdown)
		{
			--thread_pool.pool.alive;
			pthread_mutex_unlock(&thread_pool.mutex.queuelock);
			printf("thread pool close, consumers thread 0x%x exiting\n", (unsigned int)pthread_self());
			pthread_exit(NULL);
		}
		task_t task;
		int rear = thread_pool.queue.rear;
		thread_pool.queue.rear = (thread_pool.queue.rear + 1 )%thread_pool.queue.max;
		--thread_pool.queue.cur;
		++thread_pool.pool.busy;
		task.jobs = thread_pool.queue.task[rear].jobs;
		task.arg = thread_pool.queue.task[rear].arg;
		pthread_cond_signal(&thread_pool.cont.producers);
		pthread_mutex_unlock(&thread_pool.mutex.queuelock);
		task.jobs(task.arg);
		printf("1busy \n");
		pthread_mutex_lock(&thread_pool.mutex.queuelock);
		printf("2busy --\n");
		--thread_pool.pool.busy;
		pthread_mutex_unlock(&thread_pool.mutex.queuelock);
		
		printf("3 unlock\n");
//		pthread_cleanup_pop(0);
//		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
//		pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
//		pthread_testcancel();
	}
	pthread_exit(NULL);
}
