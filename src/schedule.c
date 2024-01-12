#include "schedule.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "util.h"
#include "list.h"
#include "instance.h"

/********************************************************************
 * schedule_node_t
 ********************************************************************/

/**
 * @brief Construire et initialiser un nouveau nœud d'un ordonnancement.
 * 
 * @param[in] task 
 * @param[in] bt 
 * @param[in] et 
 * @return struct schedule_node_t* 
 */
static struct schedule_node_t * new_schedule_node(struct task_t * task, unsigned long bt, unsigned long et) {
	assert(bt < et);
	assert(task != NULL);	// additional check
	struct schedule_node_t * snode = (struct schedule_node_t *)malloc(sizeof(struct schedule_node_t));

	if(snode == NULL)
		ShowMessage("Erreur d'allocation mémoire dans new_schedule_node()", 1);

	snode->task = task;
	snode->begin_time = bt;
	snode->end_time = et;

	return snode;
}

struct task_t * get_schedule_node_task(const struct schedule_node_t * snode) {
	return snode->task;
}

unsigned long get_schedule_node_begin_time(const struct schedule_node_t * snode) {
	return snode->begin_time;
}

unsigned long get_schedule_node_end_time(const struct schedule_node_t * snode) {
	return snode->end_time;
}

void set_schedule_node_begin_time(struct schedule_node_t * snode, unsigned long new_bt) {
	snode->begin_time = new_bt;
}

void set_schedule_node_end_time(struct schedule_node_t * snode, unsigned long new_et) {
	snode->end_time = new_et;
}

void view_schedule_node(const void * snode) {
	const struct schedule_node_t *node = (const struct schedule_node_t *)snode;
	printf("%lu-%lu: ", node->begin_time, node->end_time);
	view_task(node->task);
}

void delete_schedule_node(void *snode) {
	struct schedule_node_t *node = (struct schedule_node_t *)snode;
	delete_task(node->task);
	free(node);
}

/********************************************************************
 * schedule_t
 ********************************************************************/

struct schedule_t * new_schedule(int num_m) {
	assert(num_m >= 1);
	struct schedule_t * S = (struct schedule_t *)malloc(sizeof(struct schedule_t));

	if(S == NULL)
		ShowMessage("Erreur d'allocation mémoire dans new_schedule()", 1);

	S->num_machines = num_m;
	S->schedule = (struct list_t **)malloc(num_m * sizeof(struct list_t *));
	for (int i = 0; i < num_m; i++) {
		S->schedule[i] = new_list(view_schedule_node, delete_schedule_node);
	}

	return S;
}

struct list_t * get_schedule_of_machine(const struct schedule_t * S, const int machine) {
	assert(machine >= 0 && machine < S->num_machines);
	return S->schedule[machine];
}

int get_num_machines(const struct schedule_t * S) {
	return S->num_machines;
}

void view_schedule(const struct schedule_t * S) {
	printf("\n---------- schedule_t ----------\n");
	printf("Number of machines = %d\n", S->num_machines);
	for (int i = 0; i < S->num_machines; i++)
	{
		printf("Machine %d:\n", i);
		printf("=======================");
		view_list(S->schedule[i]);
	}
	printf("---- End of schedule_t view ----\n");
}

void delete_schedule(struct schedule_t * S) {
	for (int i = 0; i < S->num_machines; i++){
		delete_list(S->schedule[i], 0);	// complete deletion in main()
	}

	free(S);
}

/*
	2			<-- Nombre de machines
	M0			<-- ordonnancement de la machine M0
	0 2 01		<-- la tache 01 est exécutée de 0 à 2
	2 3 04
	3 5 01
	M1
	1 3 02
	3 6 03
*/
void save_schedule(struct schedule_t *S, const char *filename)
{
	FILE *file = fopen(filename, "w");
	if (file == NULL)
		ShowMessage("Error: cannot open file! (save_schedule)", 1);

	fprintf(file, "%d\n", S->num_machines);

	for (int i = 0; i < S->num_machines; i++) {
		fprintf(file, "M%d\n", i);
		struct list_node_t *node = S->schedule[i]->head;

		while (node != NULL) {
			struct schedule_node_t *snode = (struct schedule_node_t *)node->data;
			struct task_t *task = snode->task;

			fprintf(file, "%lu ", snode->begin_time);
			fprintf(file, "%lu ", snode->end_time);
			fprintf(file, "%s\n", task->id);

			node = node->successor;
		}
	}

	fclose(file);
}

int find_empty_machine(struct schedule_t *S, unsigned long time)
{
	// Loop over all machines in the schedule S
	for (int i = 0; i < S->num_machines; i++)
	{
		struct list_node_t *node = S->schedule[i]->head;

		// If the current machine contains no tasks, it is available
		if (node == NULL)
			return i;

		// Retrieve the last task of the current machine
		struct schedule_node_t *snode = (struct schedule_node_t *)node->data;
		while (node->successor != NULL)
		{
			node = node->successor;
			snode = (struct schedule_node_t *)node->data;
		}

		// If the last task of the current machine ends strictly before the time, it is available
		if (snode->end_time < time)
		{
			return i;
		}
	}
	// No machine is available at time, return -1
	return -1;
}

int find_machine_to_interrupt(struct schedule_t *S, unsigned long time, unsigned long processing_time)
{
	int machine_to_interrupt = -1;
	unsigned long max_remaining_processing_time = 0;

	for (int i = 0; i < S->num_machines; i++)
	{
		// Get the last task of the current machine
		struct list_node_t *node = S->schedule[i]->head;
		if (node == NULL)
			continue;

		struct schedule_node_t *last_snode = (struct schedule_node_t *)node->data;
		while (node->successor != NULL)
		{
			node = node->successor;
			last_snode = (struct schedule_node_t *)node->data;
		}

		// Check if the machine can be interrupted
		unsigned long remaining_processing_time = last_snode->end_time - time;
		if (time > last_snode->begin_time && processing_time < remaining_processing_time)
		{
			if (remaining_processing_time > max_remaining_processing_time)
			{
				max_remaining_processing_time = remaining_processing_time;
				machine_to_interrupt = i;
			}
		}
	}
	// Return the machine with the largest remaining processing time that can be interrupted at time, or -1 if none
	return machine_to_interrupt;
}

void add_task_to_schedule(struct schedule_t * S, struct task_t * task, int machine, unsigned long bt, unsigned long et) {
	
	/* Ajouter dans l’ ordonnancement S la tache task dans la machine en
	param ètre. L’exécution commence au moment **bt**(begin_time) et finit au moment **et(end_time).
	2** */
	struct schedule_node_t *schedule = new_schedule_node(task, bt, et);

	list_insert_last(S->schedule[machine], schedule);
}

unsigned long preempt_task(struct schedule_t *S, int machine, unsigned long new_et)
{
	// Get the last task on the machine
	struct list_node_t *node = S->schedule[machine]->tail;
	if (node == NULL)
		return 0;

	struct schedule_node_t *last_snode = (struct schedule_node_t *)node->data;

	// Update the end time of the last task if necessary
	unsigned long end_time = last_snode->end_time;
	if (end_time > new_et)
	{
		set_schedule_node_end_time(last_snode, new_et);
	}

	// Return the remaining processing time of the preempted task
	return end_time - new_et;
}

unsigned long get_makespan(struct schedule_t *S)
{
    unsigned long makespan = 0;
    for (int i = 0; i < S->num_machines; i++)
    {
        struct list_node_t *node = S->schedule[i]->tail;
        if (node == NULL)
            continue;

        struct schedule_node_t *snode = (struct schedule_node_t *)node->data;
        unsigned long end_time = snode->end_time;

        if (end_time > makespan)
            makespan = end_time;
    }

    return makespan;
}
