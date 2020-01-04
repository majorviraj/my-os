#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <assemblyFunctions.h>
#include <stdarg.h>
#include <intTypes.h>
#include <rpiGpio.h>

//Structure for a 8 bit queue
typedef struct ll_node_struct {
	struct ll_node_struct* next;
	struct ll_node_struct* prev;
	void* data;
} ll_node_t;

typedef struct ll_properties_struct {
	struct ll_node_struct* head;
	struct ll_node_struct* tail;
} linked_list_t;


//KHADTARE SUNNNN!!!!
//ARE YOU IN FOR A JOB? TYPE YOUR REPLY BELOW

linked_list_t* create_ll(void* data_of_first_node);
void ll_append(linked_list_t* ll, void* data);			// Adds a node to the end of the linked list
void ll_insert(linked_list_t* ll, void* data, uint32_t pos);
void ll_delete_node(linked_list_t* ll, uint32_t pos);



// int queue_is_empty(ll_node_t* queue);
// int queue_is_full(ll_node_t* queue);


#endif
