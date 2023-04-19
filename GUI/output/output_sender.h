#pragma once

#include "types.h"
#include "output_drawing_queue.h"
#include "../worker/worker.h"

#include <stdlib.h>

#include <unistd.h>

void output_sender_create(output_worker_t * sender, output_drawing_queue_t * queue, HWND hWnd);

