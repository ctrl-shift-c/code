#include <thread_pool.h>
//线程池初始化

int thread_pool_init(int poolmax, int poolmin, int queuemax)
{
	if(poolmax>=100 && poolmax<1000)
		poolmax = 100;
	if(poolmin>=10 && poolmin<=0)
		poolmin = 10;
	//所有变量赋值0
	bzero(&thread_pool, sizeof(thread_pool));
	thread_pool.pool.shutdown = 1;
	//队列大小，线程个数赋值
	thread_pool.pool.max = poolmax;
	thread_pool.pool.min = poolmin;
	thread_pool.queue.max = queuemax;
	
	//生产者个数是消费着线程的1/5
	thread_pool.pool.pronumber = poolmin / 50>0 ? poolmin / 50 : 1;
	thread_pool.pool.connumber = poolmin - thread_pool.pool.pronumber;
	int promax = poolmax/50 > 0 ? poolmax/50 : 1;
	int conmax = poolmax - promax;
	
//	thread_pool.pool.pronumber = 4;
//	int promax = 4;
//	thread_pool.pool.connumber = poolmin;
//	int conmax = poolmax;
	//初始化锁和条件变量
	USER_ERROR((
		!pthread_cond_init(&thread_pool.cond.cond_producer, NULL)  &&
		!pthread_cond_init(&thread_pool.cond.cond_consumer, NULL)  &&
		!pthread_mutex_init(&thread_pool.lock.queuelock, NULL)     &&
//		!pthread_mutex_init(&thread_pool.lock.epolllock, NULL)     &&
		!pthread_mutex_init(&thread_pool.lock.managerlock, NULL)   &&
		!pthread_mutex_init(&thread_pool.lock.probusylock, NULL)   &&
		!pthread_mutex_init(&thread_pool.lock.conbusylock, NULL)   &&
		!pthread_mutex_init(&thread_pool.lock.pronumberlock, NULL) &&
		!pthread_mutex_init(&thread_pool.lock.connumberlock, NULL)
	),"lock or mutex init failed", SHUTUP);
	//线程tid申请空间
	//生产者tid集合申请空间
	int connumber = thread_pool.pool.connumber;
    int pronumber = thread_pool.pool.pronumber;
	printf("consumers %d\npronumber %d\n", connumber, pronumber);
	printf("promax %d conmax %d\n", promax, conmax);
	USER_ERROR(
		((thread_pool.tid.producers = (pthread_t *)malloc(sizeof(pthread_t)*promax)) != NULL) ,
		"producers malloc failed" ,
		SHUTUP
	);
	bzero(thread_pool.tid.producers, sizeof(pthread_t)*promax);
	//消费者tid集合申请空间
	USER_ERROR(
        ((thread_pool.tid.consumers = (pthread_t *)malloc(sizeof(pthread_t)*conmax)) != NULL) ,
        "consumers malloc failed" ,
        SHUTUP
    );
	bzero(thread_pool.tid.consumers, sizeof(pthread_t)*conmax);
	//管理者tid集合申请空间
	USER_ERROR(
        ((thread_pool.tid.managers = (pthread_t *)malloc(sizeof(pthread_t)*2)) != NULL) ,
        "manager malloc failed" ,
        SHUTUP
    );
    bzero(thread_pool.tid.managers, sizeof(pthread_t)*2);
	//信号捕捉tid集合申请空间
	USER_ERROR(
        ((thread_pool.tid.sigaction = (pthread_t *)malloc(sizeof(pthread_t)*4)) != NULL) ,
        "sigaction malloc failed" ,
        SHUTUP
    );
    bzero(thread_pool.tid.sigaction, sizeof(pthread_t)*4);
	//线程属性申请空间
	USER_ERROR(
        ((thread_pool.attr = (pthread_attr_t *)malloc(sizeof(pthread_attr_t))) != NULL) ,
        "sigaction malloc failed" ,
        SHUTUP
    );

	//循环队列属性申请空间
    USER_ERROR(
        ((thread_pool.queue.task = (task_t *)malloc(sizeof(task_t)*queuemax)) != NULL) ,
        "sigaction malloc failed" ,
        SHUTUP
    );
	//设置线程属性为分离太
    pthread_attr_init(thread_pool.attr);
	pthread_attr_setdetachstate(thread_pool.attr, PTHREAD_CREATE_JOINABLE);
	//创建线程
	//创建捕捉线程
	
	/*
	SYSTEM_ERROR(
        pthread_create(&thread_pool.tid.sigaction[0], thread_pool.attr, thread_alive_jobs, (void *)34) == 0 ,
        "thread sigaction create failed",
        SHUTUP
    );
	*/
	SYSTEM_ERROR(
        pthread_create(&thread_pool.tid.sigaction[1], thread_pool.attr, thread_alive_jobs, (void *)35) == 0 ,
        "thread sigaction create failed",
        SHUTUP
    );

	SYSTEM_ERROR(
		pthread_create(&thread_pool.tid.sigaction[2], thread_pool.attr, thread_sigaction_jobs, (void *)SIGPIPE) == 0 ,
		"thread sigaction create failed",
		SHUTUP
	);
/*
	SYSTEM_ERROR(
        pthread_create(&thread_pool.tid.sigaction[3], thread_pool.attr, thread_alives, NULL) == 0 ,
        "thread sigaction create failed",
        SHUTUP
    );
*/

	int i;
	for(i=0; i<connumber; i++)
	{
		SYSTEM_ERROR(
			pthread_create(&thread_pool.tid.consumers[i], thread_pool.attr, thread_consumer_jobs, NULL) == 0,
			"thread consumers create failed" ,
			SHUTUP
		);
		printf("consumers create 0x%x\n", (unsigned int)thread_pool.tid.consumers[i]);
	}
	
	for(i=0; i<pronumber; i++)
    {
        SYSTEM_ERROR(
            pthread_create(&thread_pool.tid.producers[i], thread_pool.attr, thread_producer_jobs, NULL) == 0,
            "thread prosumers create failed" ,
            SHUTUP
        );
        printf("prosumers create 0x%x\n", (unsigned int)thread_pool.tid.producers[i]);
    }
	
	for(i=0; i<1; i++)
	{
		SYSTEM_ERROR(
			pthread_create(&thread_pool.tid.managers[i], thread_pool.attr, thread_manger_jobs, NULL) == 0 ,
			"thread sigaction create failed",
			SHUTUP
		);
	}

	return 1;
}
