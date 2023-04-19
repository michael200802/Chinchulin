#pragma once

#include "types.h"

void output_drawing_queue_create(output_drawing_queue_t* queue);

output_drawing_t output_drawing_queue_dequeue(output_drawing_queue_t* queue);

void output_drawing_queue_enqueue(output_drawing_queue_t* queue, output_drawing_t drawing);

void output_drawing_queue_destroy(output_drawing_queue_t* queue);
