#include <lib_queue.h>

void enqueue(queue_t* queue, uint8_t data) {

	if (queue->size == queue->max_size) {
		// printf("Queue is Full\n");
		return -1;
	}
	
	queue->pointer[queue->rear] = data;
	queue->rear++;
	queue->rear = queue->rear % queue->max_size;
	queue->size++;
}

uint8_t dequeue(queue_t* queue) {
	
	if (queue->size == 0) {
		// printf("Queue is Empty\n");
		return -1;
	}

	uint8_t tmp = queue->pointer[queue->front];
	queue->front++;
	queue->front = queue->front % queue->max_size;
	queue->size--;
	return tmp;
}

queue_t* create_queue(uint32_t size) {
	
	queue_t* new_queue = malloc(sizeof(queue_t));
	new_queue->max_size = size;
	new_queue->front = 0;
	new_queue->rear = 0;
	new_queue->size = 0;
	new_queue->pointer = malloc(sizeof(uint8_t)*size);	

	return new_queue;
}

int queue_is_full(queue_t* queue) {
	
	if (queue->size == queue->max_size) {
		return 1;
	}
	else {
		return 0;
	}
}

int queue_is_empty(queue_t* queue) {
	
	if (queue->size == 0) {
		return 1;
	}
	else {
		return 0;
	}
}
