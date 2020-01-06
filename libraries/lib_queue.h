#ifndef QUEUE_H
#define QUEUE_H

#include <assemblyFunctions.h>
#include <stdarg.h>
#include <intTypes.h>
#include <rpiGpio.h>

//Structure for a 8 bit queue
typedef struct queue_properties_struct {
	volatile uint32_t max_size;
	volatile uint32_t size;
	volatile uint32_t front;
	volatile uint32_t rear;
	volatile uint8_t* pointer; //8 bit queue
} queue_t;

queue_t* create_queue(uint32_t size);
void enqueue(queue_t* queue, uint8_t data);
uint8_t dequeue(queue_t* queue);
int queue_is_empty(queue_t* queue);
int queue_is_full(queue_t* queue);
void print_queue(queue_t* queue);

#endif