#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "util.h"
#include "tree.h"

/********************************************************************
 * list_node_t
 ********************************************************************/

/**
 * @brief
 * Construit et initialise un nouveau nœud d'une liste doublement chaînée.
 * 
 * @param[in] data Donnée à affecter au nouveau nœud.
 * @return struct list_node_t* Le nouveau nœud créé.
 */
static struct list_node_t * new_list_node(void * data) {
	struct list_node_t * node = (struct list_node_t *) malloc(sizeof(struct list_node_t));

	if(node == NULL)
		ShowMessage("Erreur d'allocation mémoire dans new_list_node()", 1);

	node->data = (struct task_t *)data;
	node->predecessor = NULL;
	node->successor = NULL;
	
	return node;
}

void * get_list_node_data(const struct list_node_t * node) {
	if(node == NULL)
		ShowMessage("Erreur dans get_list_node_data() : le nœud est NULL", 1);

	return node->data;
}

struct list_node_t * get_successor(const struct list_node_t* node) {
	if(node == NULL)
		ShowMessage("Erreur dans get_successor() : le nœud est NULL", 1);

	return node->successor;
}

struct list_node_t * get_predecessor(const struct list_node_t * node) {
	if(node == NULL)
		ShowMessage("Erreur dans get_predecessor() : le nœud est NULL", 1);

	return node->predecessor;
}

void set_list_node_data(struct list_node_t * node, void * newData) {
	if(node == NULL)
		ShowMessage("Erreur dans set_list_node_data() : le nœud est NULL", 1);

	node->data = newData;
}

void set_successor(struct list_node_t * node, struct list_node_t * newSucc) {
	if(node == NULL)
		ShowMessage("Erreur dans set_successor() : le nœud est NULL", 1);

	node->successor = newSucc;
}

void set_predecessor(struct list_node_t * node, struct list_node_t * newPred) {
	if(node == NULL)
		ShowMessage("Erreur dans set_predecessor() : le nœud est NULL", 1);
	node->predecessor = newPred;
}

/********************************************************************
 * list_t
 ********************************************************************/

struct list_t * new_list(void (*viewData)(const void*), void (*freeData)(void*)) {
	assert(viewData != NULL && freeData != NULL);
	struct list_t * L = (struct list_t *)malloc(sizeof(struct list_t));

	if(L == NULL)
		ShowMessage("Erreur d'allocation mémoire dans new_list()", 1);

	L->head = NULL;
	L->tail = NULL;
	L->numelm = 0;
	L->viewData = viewData;
	L->freeData = freeData;
	
	return L;
}

int list_is_empty(struct list_t * L) {
	// Retourne 1 si la liste est vide, 0 sinon.
	if(L == NULL){
		ShowMessage("Erreur dans list_is_empty() : la liste est NULL", 0);	// message sans arreter
		return 1;
	}

	if(L->head == NULL && L->tail == NULL)
		return 1;

	return 0;
}

int get_list_size(const struct list_t * L) {
	if(L == NULL)
		ShowMessage("Erreur dans get_list_size() : la liste est NULL", 1);

	return L->numelm;
}

struct list_node_t * get_list_head(const struct list_t * L) {
	if(L == NULL)
		ShowMessage("Erreur dans get_list_head() : la liste est NULL", 1);

	return L->head;
}

struct list_node_t * get_list_tail(const struct list_t * L) {
	if(L == NULL)
		ShowMessage("Erreur dans get_list_tail() : la liste est NULL", 1);

	return L->tail;
}

void increase_list_size(struct list_t * L) {
	L->numelm++;
}

void decrease_list_size(struct list_t * L) {
	L->numelm--;
}

void set_list_size(struct list_t * L, int newSize) {
	L->numelm = newSize;
}

void set_head(struct list_t * L, struct list_node_t * newHead) {
	L->head = newHead;
}

void set_tail(struct list_t * L, struct list_node_t * newTail) {
	L->tail = newTail;
}

void delete_list(struct list_t * L, int deleteData) {
	// If deleteData is 1, the data of each node is deleted before deleting the node 
	// else the data is not deleted.
	if (deleteData == 1)
	{
		for (struct list_node_t *E = L->head; E;)
		{
			struct list_node_t *next = E->successor;
			L->freeData(E->data);
			free(E);
			E = next;
		}
	}else{
		for (struct list_node_t *E = L->head; E;)
		{
			struct list_node_t *next = E->successor;
			free(E);
			E = next;
		}
	}
	L->head = NULL;
	L->tail = NULL;
	L->numelm = 0;
}

void view_list(const struct list_t *L)
{
	printf("\n---------- list_t ----------\n");
	printf("Number of nodes = %d\n", L->numelm);

	for (struct list_node_t *E = L->head; E;)
	{
		L->viewData(E->data); // (view_schedule_node())
		E = E->successor;
		printf("\n");
	}

	printf("---- End of list_t view ----\n");
}

void list_insert_first(struct list_t * L, void * data) {
	struct list_node_t *node = new_list_node(data);

	node->predecessor = NULL;
	node->successor = L->head;

	if (L->head != NULL)
		L->head->predecessor = node;

	L->head = node;

	if (L->tail == NULL)
		L->tail = node;

	L->numelm++;
}

void list_insert_last(struct list_t * L, void * data) {
	// Inserts a new data node data at the bottom of the list L.
	struct list_node_t *node = new_list_node(data);

	if (L->tail != NULL)
		L->tail->successor = node;

	L->tail = node;

	if (L->head == NULL)
		L->head = node;

	L->numelm++;
}

void list_insert_after(struct list_t * L, void * data, struct list_node_t * ptrelm) {
	// Inserts a new data node data after the node ptrelm in the list L.
	struct list_node_t *node = new_list_node(data);

	node->predecessor = ptrelm;
	node->successor = ptrelm->successor;

	if (ptrelm->successor != NULL)
		ptrelm->successor->predecessor = node;

	ptrelm->successor = node;

	if (L->tail == ptrelm)
		L->tail = node;

	L->numelm++;
}

void * list_remove_first(struct list_t * L) {
	assert(get_list_head(L));

	struct list_node_t *node = L->head;
	void *data = node->data;

	L->head = node->successor;

	if (L->head != NULL)
		L->head->predecessor = NULL;
	else
		L->tail = NULL;

	free(node);
	L->numelm--;

	return data;
}

void * list_remove_last(struct list_t * L) {
	assert(get_list_head(L));

	struct list_node_t *node = L->tail;
	void *data = node->data;

	L->tail = node->predecessor;

	if (L->tail != NULL)
		L->tail->successor = NULL;
	else
		L->head = NULL;

	free(node);
	L->numelm--;

	return data;
}

void * list_remove_node(struct list_t * L, struct list_node_t * node) {
	assert(get_list_head(L) && get_list_tail(L));
	/* Remove the node from the L list indicated by the node pointer and returns its data. */
	void *data = node->data;

	if (node->predecessor != NULL)
		node->predecessor->successor = node->successor;
	else
		L->head = node->successor;

	if (node->successor != NULL)
		node->successor->predecessor = node->predecessor;
	else
		L->tail = node->predecessor;
	
	free(node);
	L->numelm--;

	return data;
}