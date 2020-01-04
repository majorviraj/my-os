#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <assemblyFunctions.h>
#include <stdarg.h>
#include <intTypes.h>
#include <rpiGpio.h>
#include <lib_queue.h>
#include <lib_ll.h>

#define TASK_BLOCKED				0
#define TASK_RUNNING 				1
#define TASK_READY				2

#define SCHEDULER_MAX_TASKS_ALLOWED		100
#define SIZE_OF_THE_REGISTER_BLOCK		17
#define SCHEDULER_READY_QUEUE_SIZE		40

#define CURRENT_CPSR 				0x53
typedef struct context_stack {
	uint32_t spsr_irq;
	uint32_t sp_original_context; //of the context where you came from
	uint32_t lr_original_context; //of the context where you came from
	uint32_t r0_context;
	uint32_t r1_context;
	uint32_t r2_context;
	uint32_t r3_context;
	uint32_t r4_context;
	uint32_t r5_context;
	uint32_t r6_context;
	uint32_t r7_context;
	uint32_t r8_context;
	uint32_t r9_context;
	uint32_t r10_context;
	uint32_t r11_context;
	uint32_t r12_context;
	uint32_t pc_original_context;
} context_stack_t;

typedef struct tcb_struct {
	uint8_t task_id;
	void (*task_code_pointer)(void);
	char name[20];
	uint32_t stack_size;
	context_stack_t context; 
				//This is the context of a task stored in a format noted in context.s. 
				//It is actually copied from the IRQ stack before switching to another task. 
				//Also to load a task, the context of a task is copied into the IRQ stacl.
	uint32_t* stack_base;
	uint32_t* heap;
	uint8_t state;
	uint8_t priority;
} tcb_t;

linked_list_t* tcb_list; //A linked list of all alive tasks

uint8_t nextFreeTaskID;

tcb_t* idle_task;

queue_t* ready_task_queue;

tcb_t* get_tcb(uint8_t taskNumber);
tcb_t* rr_schedule(tcb_t* current_task);
void create_task(void (*code_pointer)(void), char* task_name, uint32_t stackInBytes);
void scheduler(uint32_t* context_stack_pointer);
#endif
