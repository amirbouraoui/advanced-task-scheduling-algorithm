#include "instance.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "util.h"
#include "list.h"

/********************************************************************
 * task_t
 ********************************************************************/

struct task_t * new_task(char * id, unsigned long proctime, unsigned long reltime) {
	// Construction et initialisation d'une nouvelle tâche.
	struct task_t * task = (struct task_t *) malloc(sizeof(struct task_t));

	// test if allocation failed
	if (task == NULL) 
		ShowMessage("Allocation failed (new_task)", 1);

	task->id = strdup(id);
	if (task->id == NULL){
		free(task);
		ShowMessage("Allocation failed (new_task)", 1);
	}

	task->processing_time = proctime;
	task->release_time = reltime;

	return task;
}

char * get_task_id(const struct task_t * task) {
	return task->id;
}

unsigned long get_task_processing_time(const struct task_t * task) {
	return task->processing_time;
}

unsigned long get_task_release_time(const struct task_t * task) {
	return task->release_time;
}

void view_task(const void * task) {
	// Afficher les caractéristiques de la tâche "task"
	struct task_t * tache = (struct task_t *) task;
	if (task == NULL){
		ShowMessage("Task is NULL : view_task()", 0);
	}else{
		printf("(%s, %lu, %lu)", tache->id, tache->processing_time, tache->release_time);
	}
}

void delete_task(void * task) {
	// Supprimer et libérer la mémoire de la tâche.
	struct task_t *t = (struct task_t *)task;
	if (t != NULL) {
		free(t->id); // strdup allocation
		free(t);
	}
}

/********************************************************************
 * Instance
 ********************************************************************/


Instance read_instance(const char * filename) {
	// Construire une nouvelle instance avec le contenu du fichier "filename".
	FILE * file = fopen(filename, "r");

	if(file == NULL)
		ShowMessage("File not found", 1);

	Instance I = new_list(view_task, delete_task);

	long temps1,temps2;
	char x[256]; // id (max 256 char)

	// verify file format
	while(fscanf(file,"%s %ld %ld", x , &temps1 , &temps2) != EOF)
	{
		struct task_t * tache = new_task(x, temps1, temps2);
		list_insert_last(I, tache);
	}

	fclose(file);

	return I;
}

void view_instance(Instance I) {
	view_list(I);
}

void delete_instance(Instance I, int deleteData) {
	delete_list(I, deleteData);
}