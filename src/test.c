#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "list.h"
#include "tree.h"
#include "instance.h"
#include "schedule.h"
#include "algo.h"

int compare_int(const void *a, const void *b) {
	return (*(int *)a - *(int *)b);
}

static int compare_lists(struct list_t *l1, int* l2[], int size) {
	if (get_list_size(l1) != size)
		return 0;

	if (get_list_head(l1) == NULL)
		return 1;

	struct list_node_t * curr = get_list_head(l1);
	int i = 0;
	while (curr != NULL) {
		if (get_list_node_data(curr) != l2[i])
			return 0;
		curr = get_successor(curr);
		i++;
	}

	curr = get_list_tail(l1);
	i = size-1;
	while (curr != NULL) {
		if (get_list_node_data(curr) != l2[i])
			return 0;
		curr = get_predecessor(curr);
		i--;
	}
	return 1;
}

void test_list_insert_last() {
	int *i1 = malloc(sizeof(int));
	int *i2 = malloc(sizeof(int));
	int *i3 = malloc(sizeof(int));
	*i1 = 1;
	*i2 = 2;
	*i3 = 3;

	struct list_t * L = new_list(viewInt, freeInt);
	int* tab[3];
	tab[0] = i1; tab[1] = i2; tab[2] = i3;

	list_insert_last(L, i1);
	int x = 0;
	if (compare_lists(L, tab, 1) == 0) printf("problème"); else x += 1;
	list_insert_last(L, i2);
	if (compare_lists(L, tab, 2) == 0) printf("problème"); else x += 1;
	list_insert_last(L, i3);
	if (compare_lists(L, tab, 3) == 0) printf("problème"); else x += 1;

	if(x == 3) printf("[PASSED] test_list_insert_last\n"); else printf("[FAILED] test_list_insert_last\n");

	// Il manque la libération de la mémoire. C'est à vous !
	delete_list(L, 1);
}

void test_tree()
{
	int passed = 1;

	struct tree_t *tree = new_tree(1, compare_int, viewInt, viewInt, freeInt, freeInt);

	int keys[] = {10, 20, 30, 40, 50, 25};
	int data[] = {100, 200, 300, 400, 500, 250};
	int n = sizeof(keys) / sizeof(keys[0]);

	for (int i = 0; i < n; i++)
	{
		int *key = malloc(sizeof(int));
		int *value = malloc(sizeof(int));
		*key = keys[i];
		*value = data[i];
		tree_insert(tree, key, value);
	}

	int search_key = 30;
	struct tree_node_t *found_node = tree_find_node(tree->root, &search_key, compare_int);
	if (!found_node)
	{
		passed = 0;
	}

	int delete_key = 25;
	void *deleted_data = tree_remove(tree, &delete_key);
	if (!deleted_data)
	{
		passed = 0;
	}
	else
	{
		freeInt(deleted_data);
	}

	delete_tree(tree, 1, 1);
	free(tree);

	if (passed)
	{
		printf("[PASSED] test_tree\n");
	}
	else
	{
		printf("[FAILED] test_tree\n");
	}
}

void test_read_instance()
{
	const char *test_instance = "./data/instance";
	Instance I = read_instance(test_instance);

	if (!I)
	{
		printf("[FAILED] test_read_instance \n");
	}
	else
	{
		printf("[PASSED] test_read_instance \n");
		delete_instance(I, 1);
	}
}

void test_create_schedule()
{
	const char *test_instance = "./data/instance";
	Instance I = read_instance(test_instance);

	if (!I)
	{
		printf("[FAILED] test_create_schedule (read instance)\n");
		return;
	}

	int m = 3;
	int preemption = 1;
	int balanced = 1;

	struct schedule_t *S = create_schedule(I, m, preemption, balanced);
	if (!S)
	{
		printf("[FAILED] test_create_schedule\n");
	}
	else
	{
		printf("[PASSED] test_create_schedule\n");
		delete_schedule(S);
	}

	delete_instance(I, 1);
}

int main()
{
	test_list_insert_last();
	test_tree();
	test_read_instance();
	test_create_schedule();
	return EXIT_SUCCESS;
}