#include <thread_pool.h>

void* thread_manger_jobs(void *arg)
{
	int shutdonw = 0;
	while(thread_pool.pool.shutdown)
	{
		sleep(1);

		//pthread_mutex_lock(&thread_pool.lock.pronumberlock);
		int max = thread_pool.pool.max;
		int min = thread_pool.pool.min;
		int promax = max/50 > 0? max/50 : 1;
		int conmax = max - promax;
		int promin = min/50 > 0 ? min/50 : 1;
		int conmin = min - promin;
//		printf("connumerberlock\n");
		pthread_mutex_lock(&thread_pool.lock.connumberlock);
//		printf("connumberlock up\n");
		int connumber = thread_pool.pool.connumber;
		int begincon = thread_pool.pool.begincon;
		pthread_mutex_unlock(&thread_pool.lock.connumberlock);
//		printf("connumberlock down\n");
//
//		printf("conbusylock\n");
		pthread_mutex_lock(&thread_pool.lock.conbusylock);
//		printf("conbusylock up\n");
		int conbusy = thread_pool.pool.conbusy;
		pthread_mutex_unlock(&thread_pool.lock.conbusylock);
//		printf("conbusylock down\n");
		//		pthread_mutex_unlock(&thread_pool.lock.pronumberlock);
//		printf("pronumberlock\n");
		pthread_mutex_lock(&thread_pool.lock.pronumberlock);
//		printf("pronumberlokc up\n");
		int pronumber = thread_pool.pool.pronumber;	
//		int beginpro = thread_pool.pool.beginpro;
		pthread_mutex_unlock(&thread_pool.lock.pronumberlock);
//		printf("pronumberlock down\n");		
/*
		if(begincon != 0)
		{
			sleep(2);	
			printf("connumberlock\n");
			pthread_mutex_lock(&thread_pool.lock.connumberlock);
//			printf("connumberlock up\n");
			int endcon = thread_pool.pool.begincon;
			int exitcon = thread_pool.pool.conexitcode;
			connumber = thread_pool.pool.connumber;
			printf("exitcon = %d endcon = %d connumber = %d\n", exitcon, endcon,connumber);
			if(endcon < 0)
			{
				endcon = 0;
				thread_pool.pool.begincon = 0;
			}
			//防止信号发送过快导致进程还没有被回收
			if(endcon == begincon && exitcon == 0)
			{
				thread_pool.pool.conexitcode = endcon;
				thread_pool.pool.connumber += endcon;
		//		thread_pool.pool.connumber += endcon;
				pthread_cond_signal(&thread_pool.cond.cond_consumer);
			}
			else if(exitcon >0)
			{
				pthread_cond_signal(&thread_pool.cond.cond_consumer);
			}
			pthread_mutex_unlock(&thread_pool.lock.connumberlock);
//			printf("conumberlock down\n");
//			
			printf("begincon %d connumber %d exitcon %d\n", begincon, connumber, exitcon);
			continue;
		}
*/
		if(begincon > 0)
		{
			sleep(1);
			/*
			pthread_mutex_lock(&thread_pool.lock.connumberlock);
//          printf("connumberlock up\n");
            int endcon = thread_pool.pool.begincon;
		//	pthread_mutex_unlock(&thread_pool.lock.connumberlock);
			if(endcon == begincon)
			{
				thread_pool.pool.begincon = 0;	
			}
			pthread_mutex_unlock(&thread_pool.lock.connumberlock);
			*/
		}
		printf("conbusy %d connumber %d begincon %d\n", conbusy, connumber, begincon);
		if(((double)conbusy *100 / connumber >= 70 && (double)connumber *100 / max < 90 && connumber < conmax)  || connumber < conmin )
		{
			if(connumber >= conmin)
			{
				int createcon = connumber - connumber/3;
				if(createcon + connumber > conmax)
					createcon = conmax - connumber;
				int createpro = (createcon + connumber)/50 - pronumber;
				if(createpro < 0)
					createpro = 0;
				if(createpro + pronumber > promax)
					createpro = promax - pronumber;
				//			printf("numbercon %d\nnumberpro %d\n", createcon, createpro);
				for(int i=0; i<conmax && createcon>0; i++)
				{
//					printf("consumer ox%x\n", (unsigned int)thread_pool.tid.consumers[i]);
					if(thread_pool.tid.consumers[i] == 0)
					{
						pthread_create(&thread_pool.tid.consumers[i], thread_pool.attr, thread_consumer_jobs, NULL);
						pthread_mutex_lock(&thread_pool.lock.connumberlock);
						++thread_pool.pool.connumber;
						pthread_mutex_unlock(&thread_pool.lock.connumberlock);
//						printf("consumer create 0x%x\n",(unsigned int)thread_pool.tid.consumers[i]);
						--createcon;
					}
				}
				promax = 0;
				for(int i=0; i<promax && createpro>0; i++)
				{
					if(thread_pool.tid.producers[i] == 0)
					{
						pthread_create(&thread_pool.tid.producers[i], thread_pool.attr, thread_producer_jobs, NULL);

						pthread_mutex_lock(&thread_pool.lock.pronumberlock);
						++thread_pool.pool.pronumber;
						pthread_mutex_unlock(&thread_pool.lock.pronumberlock);
						//					printf("producers create 0x%x\n",(unsigned int)thread_pool.tid.producers[i]);
						--createpro;
					}
				}
				
			}
			else
			{
				int createcon = conmin - connumber;
				int createpro = promin - pronumber;
				for(int i=0; i<conmax && createcon>0; i++)
                {
//                    printf("consumer ox%x\n", (unsigned int)thread_pool.tid.consumers[i]);
                    if(thread_pool.tid.consumers[i] == 0)
                    {
                        pthread_create(&thread_pool.tid.consumers[i], thread_pool.attr, thread_consumer_jobs, NULL);
                        pthread_mutex_lock(&thread_pool.lock.connumberlock);
                        ++thread_pool.pool.connumber;
                        pthread_mutex_unlock(&thread_pool.lock.connumberlock);
//                        printf("consumer create 0x%x\n",(unsigned int)thread_pool.tid.consumers[i]);
                        --createcon;
                    }
                }
				promax = 0;
                for(int i=0; i<promax && createpro>0; i++)
                {
                    if(thread_pool.tid.producers[i] == 0)
                    {
                        pthread_create(&thread_pool.tid.producers[i], thread_pool.attr, thread_producer_jobs, NULL);

                        pthread_mutex_lock(&thread_pool.lock.pronumberlock);
                        ++thread_pool.pool.pronumber;
                        pthread_mutex_unlock(&thread_pool.lock.pronumberlock);
                        //                  printf("producers create 0x%x\n",(unsigned int)thread_pool.tid.producers[i]);
                        --createpro;
                    }
                }
			}
		}
		else if((double)conbusy*100 / connumber <= 10 && connumber > conmin)
		{
			if(!shutdonw)
			{
				sleep(1);
				shutdonw = 1;
				continue;
			}
			/*
			pthread_mutex_lock(&thread_pool.lock.connumberlock);
            int connow = thread_pool.pool.connumber;
            pthread_mutex_unlock(&thread_pool.lock.connumberlock);
			if(connow != connumber)
				continue;
			*/
			int num = connumber / 2 > conmin ? connumber / 2 : connumber - conmin;
			pthread_mutex_lock(&thread_pool.lock.connumberlock);
			thread_pool.pool.conexitcode = num;
			thread_pool.pool.begincon += num;
			pthread_mutex_unlock(&thread_pool.lock.connumberlock);
			
            num = connumber - num;
			num = num/50 > 0 ? num/50 : 1;
			num = pronumber - num;
			num = 0;
			pthread_mutex_lock(&thread_pool.lock.pronumberlock);
			thread_pool.pool.proexitcode = num;
			thread_pool.pool.beginpro += num;
			pthread_mutex_unlock(&thread_pool.lock.pronumberlock);
			pthread_cond_signal(&thread_pool.cond.cond_consumer);
			shutdonw = 0;
		}
	}
	return NULL;
}
