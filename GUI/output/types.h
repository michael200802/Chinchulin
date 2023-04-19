#pragma once

#include "../../config/config.h"

#include PLT_SYS_TYPES_DIR

#include <pthread.h>

#include <windows.h>

//image of a circle and its line
typedef struct
{
        long screen_x, screen_y;
        long radius;

        long lend_x, lend_y;

        COLORREF rgb;

}output_img_t;

typedef struct
{
        output_img_t *img_arr;
        size_t img_arr_len;
}output_drawing_t;

typedef struct __output_drawing_queue_node
{
	output_drawing_t drawing;
	struct __output_drawing_queue_node * next;
}output_drawing_queue_node_t;

typedef struct __output_drawing_queue_t
{
	output_drawing_queue_node_t * head, * tail;
	pthread_mutex_t mutex;
}output_drawing_queue_t;

typedef struct
{
	output_drawing_queue_t * queue;

	HWND hWnd;

}output_sender_t;

typedef struct
{
	output_drawing_queue_t * queue;

	plt_sys_t plt_sys;

	mpfr_t time_unit;
	mpfr_t dis_unit;
}output_render_t;

