#pragma once

#include "math.h"
#include <unistd.h>
#include "types.h"
#include "output_drawing_queue.h"
#include "../worker/worker.h"
#include "../../planet_sys/plt_sys.h"

void output_render_create(output_worker_t * render, output_drawing_queue_t * queue, plt_sys_t plt_sys, mpfr_t time_unit, mpfr_t dis_unit);

