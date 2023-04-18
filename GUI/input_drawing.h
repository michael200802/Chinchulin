#pragma once

#include <windows.h>

//image of a circle and its line
typedef struct
{
	long screen_x, screen_y;
	long radius;

	long lend_x, lend_y;

	COLORREF rgb;

}input_img_t;

typedef struct
{
	input_img_t *img_arr;
	size_t img_arr_len;
}input_drawing_t;

typedef struct __input_drawing_queue_t
{
	input_drawing_t drawing;
	struct __input_drawing_queue_t * next;
}input_drawing_queue_t;



