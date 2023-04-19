#include "worker.h"

void* output_worker_routine(void*arg)
{
	output_worker_t * worker = arg;
	while(!worker->cancel)
	{
		worker->routine(worker->arg);
	}
	return NULL;
}

void output_worker_create(output_worker_t * worker, void*(*routine)(void*), void*arg)
{
	worker->routine = routine;
	worker->arg = arg;
	worker->cancel = false;

	pthread_create(&worker->thr,NULL,output_worker_routine,worker);
}

void output_worker_create_static(void*(*routine)(void*), void * arg)
{
	pthread_t thr;
	pthread_create(&thr,NULL,routine,arg);
}

void output_worker_destroy(output_worker_t * worker)
{
	worker->cancel = true;
	pthread_join(worker->thr,NULL);
}
