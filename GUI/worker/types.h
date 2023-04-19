#pragma once

#include "../../config/config.h"

#include PLT_SYS_TYPES_DIR

#include <pthread.h>

#include <stdbool.h>

#include <stdatomic.h>

typedef struct
{
	void*(*routine)(void*);
	void*arg;
	_Atomic(bool) cancel;

	pthread_t thr;
}output_worker_t;

