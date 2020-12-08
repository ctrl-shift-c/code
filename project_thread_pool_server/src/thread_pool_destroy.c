#include <thread_pool_server.h>
#include <mysocket_api.h>
int thread_pool_destroy(thread_pool_t *ptr)
{
	pthread_mutex_destroy(&acclock);
	pthread_mutex_destroy(&ptr->queuelock);
	pthread_mutex_destroy(&epolllock);
	pthread_cond_destroy(&ptr->not_full);
	pthread_cond_destroy(&ptr->not_empty);
	free(ptr->customer_id);
	free(ptr->producer_id);
	free(ptr);
	return 0;
}
