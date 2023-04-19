#include "output_drawing_queue.h"

void output_drawing_queue_create(output_drawing_queue_t* queue)
{
	pthread_mutex_init(&queue->mutex,NULL);
	queue->head = queue->tail = NULL;
}

output_drawing_t output_drawing_queue_dequeue(output_drawing_queue_t* queue)
{
	pthread_mutex_lock(&queue->mutex);

	if(queue->tail == NULL)
	{
		return (output_drawing_t){};
	}

	output_drawing_t drawing = queue->tail->drawing;
	output_drawing_queue_node_t * node_to_kill = queue->tail;

	queue->tail = queue->tail->next;

	free(node_to_kill);

	pthread_mutex_unlock(&queue->mutex);

	return drawing;
}

void output_drawing_queue_enqueue(output_drawing_queue_t* queue, output_drawing_t drawing)
{
	pthread_mutex_lock(&queue->mutex);

	output_drawing_queue_node_t * node = malloc(sizeof(output_drawing_queue_node_t));
	node->drawing = drawing;
	node->next = queue->tail;

	queue->tail = node;
	if(queue->head == NULL)
	{
		queue->head = queue->tail;
	}

	pthread_mutex_unlock(&queue->mutex);
}

void output_drawing_queue_destroy(output_drawing_queue_t* queue)
{
	output_drawing_queue_node_t * node = queue->tail, * next_node;
	while(node)
	{
		next_node = node->next;
		free(node);
		node = next_node;
	}
}


