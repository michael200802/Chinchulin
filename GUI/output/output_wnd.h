#pragma once
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

#include <semaphore.h>

#include "../worker/worker.h"
#include "output_drawing_queue.h"
#include "output_render.h"
#include "output_sender.h"

void output_start(plt_sys_t plt_sys,mpfr_t time_unit,mpfr_t dis_unit);
