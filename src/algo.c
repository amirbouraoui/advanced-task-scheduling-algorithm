#include "algo.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <limits.h>

#include "util.h"
#include "list.h"
#include "tree.h"
#include "instance.h"
#include "schedule.h"

/********************************************************************
 * event_key_t
 ********************************************************************/

/**
 * @brief 
 * Une structure qui definit la clé de l'ensemble des événements.
 */
struct event_key_t {
	int event_type; // 0 si l'événement correspond à la libération d'une tâche,
					// 1 si l'événement correspond à la fin d'exécution d'une tâche
	unsigned long event_time;
	unsigned long processing_time; // par convention, 0 si l'événement correspond à la fin d'exécution d'une tâche
								   // sinon, la durée opératoire de la tâche qui va être libérée
	char * task_id;
	int machine; // par convention, 0 si l'événement correspond à la libération d'une tâche,
				 // sinon, la machine qui exécute la tâche de l'événement
};

/**
 * @brief 
 * 
 * @param[in] event_type 
 * @param[in] event_time 
 * @param[in] processing_time 
 * @param[in] task_id 
 * @param[in] machine 
 * @return struct event_key_t* 
 */
struct event_key_t * new_event_key(int event_type, unsigned long event_time, unsigned long processing_time, char * task_id, int machine) {
	struct event_key_t * event = (struct event_key_t *)malloc(sizeof(struct event_key_t));
	if (event == NULL)
		ShowMessage("Not enough memory! (new_event_key)", 1);

	event->event_type = event_type;
	event->event_time = event_time;
	event->processing_time = processing_time;
	event->task_id = strdup(task_id);
	event->machine = machine;

	if (event->task_id == NULL)
		ShowMessage("Not enough memory! (new_event_key: strdup)", 1);

	return event;
}

/**
 * @brief 
 * 
 * @param[in] key 
 * @return int 
 */
int get_event_type(const struct event_key_t * key) {
	return key->event_type;
}

/**
 * @brief 
 * 
 * @param[in] key 
 * @return unsigned long 
 */
unsigned long get_event_time(const struct event_key_t * key) {
	return key->event_time;
}

/**
 * @brief 
 * 
 * @param[in] key 
 * @return unsigned long 
 */
unsigned long get_event_processing_time(const struct event_key_t * key) {
	return key->processing_time;
}

/**
 * @brief 
 * 
 * @param[in] key 
 * @return char* 
 */
char * get_event_task_id(const struct event_key_t * key) {
	return key->task_id;
}

/**
 * @brief 
 * 
 * @param[in] key 
 * @return int 
 */
int get_event_machine(const struct event_key_t * key) {
	return key->machine;
}

/**
 * @brief 
 * 
 * @param[in] key 
 */
void view_event_key(const void *key)
{
    struct event_key_t *k = (struct event_key_t *)key;

    if (k->event_type == 0)
    {
        printf("EVENT_TYPE: RELEASE TASK\t");
    }
    else if (k->event_type == 1)
    {
        printf("EVENT_TYPE: END TASK\t");
    }
    else
    {
        printf("EVENT_TYPE: INVALID\t");
    }

    printf("EVENT_TIME: %lu\t", k->event_time);

    if (k->event_type == 0)
    {
        printf("PROCESSING_TIME: %lu\t", k->processing_time);
    }

    if (k->task_id == NULL)
    {
        printf("TASK_ID: (null)\t");
    }
    else
    {
        printf("TASK_ID: %s\t", k->task_id);
    }

    if (k->event_type == 1)
    {
        printf("MACHINE: %d\t", k->machine);
    }

    printf("\n");
}

/**
 * @brief 
 * 
 * @param[in] key 
 */
void delete_event_key(void * key) {
	struct event_key_t * k = (struct event_key_t *)key;

	free(k->task_id);
	free(k);
}

/**
 * @brief Comparer deux événements \p a et \p b.
 * Les règles de comparaison sont données dans la présentation du cours.
 * 
 * @param[in] a 
 * @param[in] b 
 * @return int 
 */
int event_preceed(const void *a, const void *b)
{
	const struct event_key_t *A = a;
	const struct event_key_t *B = b;

	if (A->event_time < B->event_time)
		return 1;
	else if (A->event_time > B->event_time)
		return 0;

	if (A->event_type < B->event_type)
		return 1;
	else if (A->event_type > B->event_type)
		return 0;

	if (A->processing_time < B->processing_time)
		return 1;
	else if (A->processing_time > B->processing_time)
		return 0;

	return strcmp(A->task_id, B->task_id) < 0;
}

/********************************************************************
 * ready_task_key_t
 ********************************************************************/

/**
 * @brief
 * Une structure qui définit la clé de la file d'attente avec les "ready tasks".
 */
struct ready_task_key_t {
	unsigned long remaining_processing_time;
	char * task_id;
};

/**
 * @brief 
 * 
 * @param[in] remaining_processing_time 
 * @param[in] task_id 
 * @return struct ready_task_key_t* 
 */
struct ready_task_key_t * new_ready_task_key(unsigned long remaining_processing_time, char * task_id) {
	struct ready_task_key_t *key = (struct ready_task_key_t *)malloc(sizeof(struct ready_task_key_t));

	if(key == NULL)
		ShowMessage("Not enough memory! (new_ready_task_key: malloc)", 1);

	key->remaining_processing_time = remaining_processing_time;
	key->task_id = strdup(task_id);
	if (key->task_id == NULL)
		ShowMessage("Not enough memory! (new_ready_task_key: strdup)", 1);

	return key;
}

/**
 * @brief 
 * 
 * @param[in] key 
 * @return unsigned long 
 */
unsigned long get_ready_task_remaining_processing_time(const struct ready_task_key_t * key) {
	return key->remaining_processing_time;
}

/**
 * @brief 
 * 
 * @param[in] key 
 * @return char* 
 */
char * get_ready_task_id(const struct ready_task_key_t * key) {
	return key->task_id;
}

/**
 * @brief 
 * 
 * @param[in] key 
 */
void view_ready_task_key(const void * key) {
	struct ready_task_key_t * k = (struct ready_task_key_t *)key;

	printf("REMAINING_PROCESSING_TIME: %lu\t", k->remaining_processing_time);
	printf("TASK_ID: %s\t", k->task_id);
	printf("\n");
}

/**
 * @brief 
 * 
 * @param[in] key 
 */
void delete_ready_task_key(void * key) {
	struct ready_task_key_t * k = (struct ready_task_key_t *)key;

	free(k->task_id);
	free(k);
}

/**
 * @brief Comparer deux tâches \p a et \p b de la file d'attente afin de définir la tâche la plus prioritaire.
 * Les deux paramètres d'entrée \p a et \p b sont de type struct ready_task_key_t *.
 * Les règles de comparaison sont données dans la présentation du cours.
 *
 * @param[in] a 
 * @param[in] b 
 * @return int 
 */
int ready_task_preceed(const void * a, const void * b) {
	struct ready_task_key_t *key_a = (struct ready_task_key_t *)a;
	struct ready_task_key_t *key_b = (struct ready_task_key_t *)b;

	if (key_a->remaining_processing_time > key_b->remaining_processing_time) {
		return 1;
	}
	else if (key_a->remaining_processing_time == key_b->remaining_processing_time)
	{
		int id_cmp = strcmp(key_a->task_id, key_b->task_id);
		if (id_cmp < 0)
		{
			return 1;
		}
		else if (id_cmp > 0)
		{
			return -1;
		}
	}
	return 0;
}

// Helper functions
struct task_t *get_task(Instance I, const char *task_id)
{
	if (I == NULL || task_id == NULL)
		ShowMessage("get_task: I is NULL", 1);

	for (struct list_node_t *Noeud = I->head; Noeud != NULL; Noeud = Noeud->successor)
	{
		struct task_t *Tache = Noeud->data;
		if (strcmp(Tache->id, task_id) == 0)
		{
			return Tache;
		}
	}
	return NULL;
}
struct task_t *get_current_task(struct schedule_t *S, int machine, unsigned long time)
{
	struct list_t *machine_schedule = S->schedule[machine];
	struct list_node_t *node = machine_schedule->head;

	while (node != NULL)
	{
		struct schedule_node_t *snode = node->data;
		if (snode->begin_time <= time && snode->end_time >= time)
		{
			return snode->task;
		}
		node = node->successor;
	}

	return NULL;
}
// Helper functions


/********************************************************************
 * The algorithm
 ********************************************************************/
struct schedule_t *create_schedule(Instance I, int num_m, int preemption, int balanced_tree)
{
	if (list_is_empty(I) || num_m <= 0)
		ShowMessage("create_schedule: Invalid input parameters", 1);

	// 1: Initialisation :
	// – Schre = nouveau ordonnancement vide
	struct schedule_t *Schre = new_schedule(num_m);

	// – Queue = file d’attente de ready tasks vide
	struct tree_t *Queue = new_tree(balanced_tree, ready_task_preceed, view_event_key, view_task, delete_event_key, delete_task);

	// – Even = ensemble des evenements initialise avec les dates de liberation de toutes les taches
	struct tree_t *Even = new_tree(balanced_tree, event_preceed, view_event_key, view_task, delete_event_key, delete_task);

	// For each Tache, add the release Even to the Even set Even
	for (struct list_node_t *Noeud = I->head; Noeud != NULL; Noeud = Noeud->successor){
		struct task_t *Tache = Noeud->data;
		struct event_key_t *release_event = new_event_key(0, Tache->release_time, Tache->processing_time, Tache->id, 0);
		tree_insert(Even, release_event, Tache);
	}

	// 2: tant que Even n’est pas vide faire
	while (!tree_is_empty(Even))
	{
		// 3: Extraire de Even l’evenement Even correspondant a l’instant t le plus petit
		struct tree_node_t *min_node = tree_min(Even->root);
		struct event_key_t *min_event_key = min_node->key;

		int event_type = get_event_type(min_event_key);

		if (event_type == 0)
		{
			// 4: si Even concerne la liberation d’une tache Tj alors
			struct task_t *Tache = get_task(I, get_event_task_id(min_event_key));
			tree_remove(Even, min_event_key);

			// 5: si il existe une machine Mi libre en t alors
			int machine = find_empty_machine(Schre, Tache->release_time);
			if (machine != -1 && (tree_is_empty(Queue) || Tache->processing_time <= get_task_processing_time(tree_min(Queue->root)->key)))
			{
				// 6: Affecter Tj a Mi en t dans Schre
				long current_time = Tache->release_time;
				add_task_to_schedule(Schre, Tache, machine, current_time, current_time + Tache->processing_time);

				// 7: Ajouter dans Even l’evenement fin d’execution de la tache Tj (End alors processing_time = 0 (à debugger mais logiquement c'est 0))
				struct event_key_t *FinEvent = new_event_key(1, current_time + Tache->processing_time, Tache->processing_time, Tache->id, machine);
				tree_insert(Even, FinEvent, Tache);
			}
			else if (preemption && (machine = find_machine_to_interrupt(Schre, Tache->release_time, Tache->processing_time)) != -1)
			{
				// 8: si il existe une machine Mi qui execute une tache Tk a l’instant t tel que pj < qk(t) alors
				struct task_t *Tk = get_current_task(Schre, machine, Tache->release_time);

				// 9: Preempter la tache Tk (mettre a jour la date de fin de la sous-partie exécutée sur l’ordonnancement Schre)
				long remaining_processing_time = preempt_task(Schre, machine, Tache->release_time);

				// 10: Supprimer l’evenement concernant la fin de Tk
				struct event_key_t *preempted_task_event_key = new_event_key(1, Tk->processing_time + Tk->release_time, Tk->processing_time, Tk->id, machine);
				tree_remove(Even, preempted_task_event_key);

				// 11: Ajouter Tk dans la file d’attente Queue avec son qk(t) (et pas son pk)
				struct ready_task_key_t *ready_task = new_ready_task_key(remaining_processing_time, Tk->id);
				tree_insert(Queue, ready_task, Tk);

				// 12: Affecter Tj a Mi en t dans Schre
				add_task_to_schedule(Schre, Tache, machine, Tache->release_time, Tache->release_time + Tache->processing_time);

				// 13: Ajouter dans Even l’evenement fin d’execution de la tache Tj
				struct event_key_t *FinEvent = new_event_key(1, Tache->release_time + Tache->processing_time, Tache->processing_time, Tache->id, machine);
				tree_insert(Even, FinEvent, Tache);
			}
			else
			{
				// 14: sinon
				// 15: Ajouter Tj dans la file d’attente Queue
				struct ready_task_key_t *ready_task = new_ready_task_key(Tache->processing_time, Tache->id);
				tree_insert(Queue, ready_task, Tache);
			}
		}
		else if (event_type == 1)
		{
			// 16: sinon si Even concerne la fin d’execution d’une tache Tj sur Mi alors
			int machine = get_event_machine(min_event_key);
			long current_time = get_event_time(min_event_key);
			tree_remove(Even, min_event_key);

			// 17: si la file d’attente Queue n’est pas vide alors
			if (!tree_is_empty(Queue))
			{
				// 18: Extraire de Queue la tache Tk avec la durée la plus courte (règle SPT)
				struct tree_node_t *ready_node = tree_min(Queue->root);
				struct ready_task_key_t *ready_task = get_tree_node_key(ready_node);
				struct task_t *Tk = get_task(I, get_ready_task_id(ready_task));

				tree_remove(Queue, ready_task);

				// 19: Affecter Tk a Mi en t dans Schre
				add_task_to_schedule(Schre, Tk, machine, current_time, current_time + Tk->processing_time);

				// 20: Ajouter dans Even l’evenement fin d’execution de la tache Tk
				struct event_key_t *FinEvent = new_event_key(1, current_time + Tk->processing_time, Tk->processing_time, Tk->id, machine);
				tree_insert(Even, FinEvent, Tk);
			}
		}else{
			ShowMessage("Invalid event type! (create_schedule)", 1);
		}
	}
	
	// Free the memory used by the Even set Even and the ready Tache queue Queue
	delete_tree(Even, 0,0);
	delete_tree(Queue, 0,0);
	
	return Schre;
}
