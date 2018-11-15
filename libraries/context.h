/*
 * file: context.h
 */ 

#ifndef CONTEXT_H
#define CONTEXT_H

#include <intTypes.h>

typedef enum {
	RUNNING, BLOCKED
}states_e;

volatile typedef struct {
	uint32_t id;
	void (*process_function)(intptr_t*);
	uint32_t* stack_base;
	uint32_t processors_registors[13]; // r0-r12
	uint32_t stack_pointer;
	uint32_t link_register;
	uint32_t program_counter;
	uint32_t cpsr;
	states_e state;
}__attribute__((packed)) process_control_block_t;

process_control_block_t process_control_block[3]__attribute__((packed));
	states_e state2;
#endif			/* CONTEXT_H */
