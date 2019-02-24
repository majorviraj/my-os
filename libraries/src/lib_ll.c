#include <lib_ll.h>
#include <stdOutput.h>

linked_list_t *create_ll(void* data_of_first_node)
{
	linked_list_t *ll = malloc(sizeof(linked_list_t));
	ll->head = malloc(sizeof(ll_node_t));
	ll->tail = ll->head;
	ll->head->data = data_of_first_node;
	ll->head->next = 0;
	ll->head->prev = 0;

	return ll;
}

void ll_append(linked_list_t *ll, void *data)
{
	ll_node_t* new_node = malloc(sizeof(ll_node_t));
	new_node->data = data;
	ll->tail->next = new_node;
	new_node->next = 0;
	new_node->prev = ll->tail;
	ll->tail = new_node;
}

void ll_delete_node(linked_list_t* ll, uint32_t pos) {
	if (pos == 0) {
		ll->head=ll->head->next;
		free(ll->head);

	}
	else {
		ll_node_t* tmp = ll->head;
		for(int i=0; i<pos; (i++) && (tmp = tmp->next));
		tmp->prev->next = tmp->next;
		tmp->next->prev = tmp->prev;
		free(tmp);
	}
}

void ll_insert(linked_list_t* ll, void* data, uint32_t pos) {
	ll_node_t* tmp = ll->head;
	for(int i=0; i<pos; (i++) && (tmp = tmp->next));
	ll_node_t* new_node = malloc(sizeof(ll_node_t));

	new_node->prev = tmp->prev;
	new_node->next = tmp;
	tmp->prev->next = new_node;
	tmp->prev = new_node;
}
