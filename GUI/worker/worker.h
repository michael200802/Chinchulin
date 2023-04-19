#include "types.h"

//routine is inside a loop
void output_worker_create(output_worker_t * worker, void*(*routine)(void*), void * arg);

//routine is the loop
void output_worker_create_static(void*(*routine)(void*), void * arg);

void output_worker_destroy(output_worker_t * worker);
