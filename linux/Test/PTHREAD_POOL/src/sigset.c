#include <thread_pool.h>

void sigset()
{
	sigset_t sig;
	sigemptyset(&sig);
	sigaddset(&sig, SIGPIPE);
	pthread_sigmask(SIG_BLOCK, &sig, NULL);
}
