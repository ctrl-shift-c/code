#include <thread_pool.h>

void siga_jobs(int arg)
{
	int promax = thread_pool.pool.max/50 >0 ? thread_pool.pool.max/50 : 1;
	int conmax = thread_pool.pool.max - promax;
	int flags = 1;
	int err;
	/*
	if(arg == 34)
	{
		while(flags)
		{
			for(int i=0; i<promax; i++)
			{
				if(thread_pool.tid.producers[i] == 0)continue;
				
				err = pthread_tryjoin_np(thread_pool.tid.producers[i], NULL);
				
				err = pthread_kill(thread_pool.tid.producers[i], 0);
				if(ESRCH == err)
				{
					pthread_join(thread_pool.tid.producers[i], NULL);
//					printf("promax tryjoin successly 0x%x\n", (unsigned int)thread_pool.tid.producers[i]);
					thread_pool.tid.producers[i] = 0;

					pthread_mutex_lock(&thread_pool.lock.pronumberlock);
					--thread_pool.pool.beginpro;
					pthread_mutex_unlock(&thread_pool.lock.pronumberlock);
					break;
				}
			}
		}
	}
	*/
	if(arg == 35)
	{
		while(flags)
		{
			for(int i=0; i<conmax; i++)
            {
                if(thread_pool.tid.consumers[i] == 0)
				{
					kil++;
					continue;
				}
				/*
                err = pthread_tryjoin_np(thread_pool.tid.consumers[i], NULL);
				*/
				err = pthread_kill(thread_pool.tid.consumers[i], 0);
				if(err == 0)
				{
					continue;
				}
				else
				{
//					printf("ESRCH\n");
                    int join = pthread_join(thread_pool.tid.consumers[i], NULL);
					if(join != 0)
					{
						perror("join error");
					}
					pthread_mutex_lock(&thread_pool.lock.connumberlock);
  //                  printf("conmax tryjoin successly 0x%x\n", (unsigned int)thread_pool.tid.consumers[i]);
					thread_pool.tid.consumers[i] = 0;
                    --thread_pool.pool.begincon;
                    pthread_mutex_unlock(&thread_pool.lock.connumberlock);
					break;
                }
            }
		//	printf("kil = %d\n", kil);
		//	sleep(1);
		}
	}
}

void* thread_alive_jobs(void *arg)
{
	int sign = (long)arg;
	while(thread_pool.pool.shutdown)
    {
		struct sigaction sig;
		bzero(&sig, sizeof(sig));
		sig.sa_handler = siga_jobs;
		sig.sa_flags = 0;
		sigemptyset(&sig.sa_mask);
		sigaction(sign, &sig, NULL);
		/*
        int err = pthread_kill(tid , 0);
        if(ESRCH == err || err == -1)
            return 0;
		return 1;
		*/
    }
    return NULL;
}
