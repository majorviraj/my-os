#include <scheduler.h>
#include <lib_queue.h>
#include <stdOutput.h>
#include <lib_ll.h>

extern _inf_loop;
tcb_t *current_task;
void scheduler_init()
{
	nextFreeTaskID = 0;
	ready_task_queue = create_queue(SCHEDULER_READY_QUEUE_SIZE);
	
	//Creating Idle Task
	// tcb_t *idle_task = malloc(sizeof(tcb_t));
	
	idle_task = malloc(sizeof(tcb_t));
	tcb_list = create_ll(idle_task);


	
	memcpy(&idle_task->name, "idle_task", sizeof("idle_task"));
	idle_task->task_code_pointer = _inf_loop;
	idle_task->task_id = nextFreeTaskID++; //Increment next free task id
	idle_task->stack_size = 0;
	idle_task->stack_base = malloc(1);
	idle_task->state = TASK_READY;

	//Create initial context of new task
	idle_task->context.pc_original_context = _inf_loop;
	idle_task->context.sp_original_context = idle_task->stack_base;
	idle_task->context.spsr_irq = 0x40000053; //SVR mode with IRQ enabled, FIQ disabled and flags 0

	//add newly created task to the ready queue
	enqueue(ready_task_queue, idle_task->task_id);
	

}

void scheduler(uint32_t* context_stack_pointer)
{

	//save context of current task
	memcpy((void *)&current_task->context, (context_stack_pointer), SIZE_OF_THE_REGISTER_BLOCK);

	printf("\nCurrent context : %s ID= %i ", current_task->name, current_task->task_id);
	for(uint32_t i = 0; i< SIZE_OF_THE_REGISTER_BLOCK; i++) {
		printf("%x\t", *(context_stack_pointer + i));
		if (i%8 == 0) {
			printf("\n");
		}
	}

	

	current_task = rr_schedule(current_task);

	//load context of new task
	memcpy((context_stack_pointer), (void *)&current_task->context, SIZE_OF_THE_REGISTER_BLOCK);

	printf("\nNew context %s ID= %i ", current_task->name, current_task->task_id);
	for(uint32_t i = 0; i< SIZE_OF_THE_REGISTER_BLOCK; i++) {
		printf("%x\t", *(context_stack_pointer + i));
		if (i%8 == 0) {
			printf("\n");
		}
	}

	printf("\n");

}

void create_task(void (*code_pointer)(void), char *task_name, uint32_t stackInBytes)
{

	tcb_t *newTask = malloc(sizeof(tcb_t));
	uint32_t len_temp = strlen(task_name);
	if (len_temp >= 20) {
		memcpy(&newTask->name, task_name, 20);
	}
	else
	{
		memcpy(&newTask->name, task_name, len_temp);
	}
	
	newTask->task_code_pointer = code_pointer;
	newTask->task_id = nextFreeTaskID++; //Increment next free task id
	newTask->stack_size = stackInBytes;
	newTask->stack_base = malloc(stackInBytes);
	newTask->state = TASK_READY;

	//Create initial context of new task
	newTask->context.pc_original_context = code_pointer;
	newTask->context.sp_original_context = newTask->stack_base;
	newTask->context.spsr_irq = 0x53; //SVR mode with IRQ enabled, FIQ disabled and flags 0

	//add newly created task to the ready queue
	enqueue(ready_task_queue, newTask->task_id);

	//add newly created task to the task list. i.e the tcb_list linked list.
	ll_append(tcb_list, (void *)newTask);
}

tcb_t* get_tcb(uint8_t taskNumber)
{
	ll_node_t* task;
	ll_node_t* tempNode;
	tcb_t* temp;

	if (taskNumber == 0)
	{
		return idle_task;
	}
	else
	{
		tempNode = tcb_list->head->next;
		while (tempNode != tcb_list->tail) {
			temp = (tcb_t *)tempNode->data;
			if (temp->task_id == taskNumber) {
				return temp;
			}

			tempNode = tempNode->next;
		}
		return NULL;
	}
}

tcb_t* rr_schedule(tcb_t* current_task)
{
	//Dequeue new task from ready queue
	//Change state of new task to running
	//Change state of current task to ready
	//Enque current task to ready queue
	//return new task
	
	uint8_t nextTaskID = dequeue(ready_task_queue);
	tcb_t* new_task = get_tcb(nextTaskID);
	new_task->state = TASK_RUNNING;
	current_task->state = TASK_READY;
	enqueue(ready_task_queue, current_task->task_id);

	return new_task;

}
