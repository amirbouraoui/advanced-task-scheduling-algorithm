#ifndef _SCHEDULE_H_
#define _SCHEDULE_H_

/**
 * @brief 
 * Le fichier définit la structure d'un ordonnancement des tâches.
 */

/**
 * @brief 
 * Un nœud d'un ordonnancement correspond à l’exécution (partielle ou pas) d'une tâche et contient
 * (+) la tâche à exécuter (task),
 * (+) le début de l’exécution (begin_time), et
 * (+) la fin de l’exécution (end_time).
 * Dans le cas d'un ordonnancement préemptif, une tâche peut se retrouver dans plusieurs nœuds.
 * La somme de (end_time - begin_time) de ces nœuds doit être égal à la durée de cette tâche.
 */
struct schedule_node_t {
	struct task_t * task;
	unsigned long begin_time;
	unsigned long end_time;
};

/**
 * @brief Restitue la tâche du nœud \p snode.
 * 
 * @param[in] snode 
 * @return struct task_t* 
 */
struct task_t * get_schedule_node_task(const struct schedule_node_t * snode);

/**
 * @brief Restitue la date de début du nœud \p snode.
 * 
 * @param[in] snode 
 * @return unsigned long 
 */
unsigned long get_schedule_node_begin_time(const struct schedule_node_t * snode);


/**
 * @brief Restitue la date de fin du nœud \p snode.
 * 
 * @param[in] snode 
 * @return unsigned long 
 */
unsigned long get_schedule_node_end_time(const struct schedule_node_t * snode);

/**
 * @brief Remplace la date de début du nœud \p snode par \p new_bt.
 * 
 * @param[in] snode 
 * @param new_bt 
 */
void set_schedule_node_begin_time(struct schedule_node_t * snode, unsigned long new_bt);

/**
 * @brief Remplace la date de fin du nœud \p snode par \p new_bt.
 * 
 * @param[in] snode 
 * @param new_et 
 */
void set_schedule_node_end_time(struct schedule_node_t * snode, unsigned long new_et);

/**
 * @brief Afficher les caractéristiques du nœud d'ordonnancement \p snode.
 * 
 * @param[in] snode 
 */
void view_schedule_node(const void * snode);

/**
 * @brief Supprimer et libérer la mémoire du nœud d'ordonnancement \p snode.
 * Attention ! la mémoire de l'attribut task du nœud \p snode ne doit pas être libérée. 
 * 
 * @param[in] snode 
 */
void delete_schedule_node(void * snode);

/**
 * @brief 
 * Un ordonnancement est
 * (+) un tableau de listes (schedule) chacune correspondant à ordonnancement d'une machine, et
 * (+) le nombre de machines de l'ordonnancement (num_machines).
 * 
 * NB : l'ordonnancement de chaque machine est une liste qui contient l'enchaînement d’exécution
 * de tâches dans l'ordre croissant par rapport à leur date de début (begin_time dans schedule_node_t).
 */
struct schedule_t {
    struct list_t ** schedule;
    int num_machines;
};

/**
 * @brief Construire et initialiser un nouveau ordonnancement vide avec \p num_m machines.
 * 
 * @param[in] num_m 
 * @return struct schedule_t* 
 */
struct schedule_t * new_schedule(int num_m);

/**
 * @brief Restitue le sous-ordonnancement de la machine \p machine de l'ordonnancement \p S.
 * 
 * @param[in] S 
 * @param[in] machine 
 * @return struct list_t* 
 */
struct list_t * get_schedule_of_machine(const struct schedule_t * S, const int machine);

/**
 * @brief Restitue le nombre de machines de l'ordonnancement \p S.
 * 
 * @param[in] S 
 * @return int 
 */
int get_num_machines(const struct schedule_t * S);

/**
 * @brief Afficher l'ordonnancement \p S.
 * 
 * @param[in] S 
 */
void view_schedule(const struct schedule_t * S);

/**
 * @brief Supprimer et libérer la mémoire de l'ordonnancement \p S.
 * 
 * @param[in] S 
 */
void delete_schedule(struct schedule_t * S);

/**
 * @brief Enregistre l'ordonnancement \p S au fichier \p filename.
 * 
 * @param[in] S 
 * @param[in] filename
 */
void save_schedule(struct schedule_t * S, const char * filename);

/**
 * @brief Restitue la première machine de l'ordonnancement \p S qui est disponible au moment \p time.
 * Si aucune machine n'est disponible en \p time, la valeur retournée est -1.
 *
 * NB1 : Il faut chercher les machines de façon itérative en commençant par la première.
 * La machine disponible avec le plus petit indice doit être restituée.
 * 
 * NB2 : Afin de décider si une machine est disponible au moment \p time, il suffit de comparer \p time
 * avec le end_time du dernier nœud de cette machine (il n'y a pas besoin de parcourir tous les nœuds).
 * 
 * NB3 : Une machine n'est pas considérée comme disponible au moment \p time si il y a une tâche qui
 * termine son exécution en ce moment (c'est-à-dire, si end_time=time).
 *
 * @param[in] S 
 * @param[in] time 
 * @return int 
 */
int find_empty_machine(struct schedule_t * S, unsigned long time);

/**
 * @brief Restitue la première machine de l'ordonnancement \p S qui peut être interrompue au moment \p time.
 * Si aucune machine ne peut pas être interrompue en \p time, la valeur retournée est -1.
 *
 * NB1 : Il faut chercher les machines de façon itérative en commençant par la première.
 * La machine à interrompre avec le plus petit indice doit être restituée.
 *
 * NB2 : Afin de décider si une machine est susceptible à être interrompue au moment \p time,
 * il suffit de comparer le temps d’exécution \p processing_time de la tâche qu'on veut commencer
 * avec le temps restant ( \p time - begin_time ) de la tâche qui est en train d’exécuter
 * (dernier nœud de cette machine, il n'y a pas besoin de parcourir tous les nœuds).
 * 
 * @param[in] S 
 * @param[in] time 
 * @param[in] processing_time 
 * @return int 
 */
int find_machine_to_interrupt(struct schedule_t * S, unsigned long time, unsigned long processing_time);

/**
 * @brief Ajouter dans l'ordonnancement \p S la tâche \p task dans la machine \p machine.
 * L’exécution commence au moment \p bt et finit au moment \p et.
 *
 * NB1 : il faut s'assurer avant l'appel de cette procédure que la machine \p machine est libre au moment \p bt,
 * et qu'il n'y a pas d'autre tâche ordonnancée après cette date.
 * 
 * @param[in] S 
 * @param[in] task 
 * @param[in] machine 
 * @param[in] bt 
 * @param[in] et 
 */
void add_task_to_schedule(struct schedule_t * S, struct task_t * task,
							int machine, unsigned long bt, unsigned long et);

/**
 * @brief Mettre à jour la date de fin (end_time) de la dernière tâche de la machine \p machine
 * de l'ordonnancement \p S avec la valeur \p new_et.
 * 
 * @param[in] S 
 * @param[in] machine 
 * @param[in] new_et 
 * @return unsigned long 
 */
unsigned long preempt_task(struct schedule_t * S, int machine, unsigned long new_et);

/**
 * @brief Calculer et renvoyer la valeur de l'objectif "makespan" de l'ordonnancement \p S.
 * Le "makespan" est la date de fin de la dernière tâche de l'ordonnancement.
 * 
 * @param[in] S 
 * @return unsigned long 
 */
unsigned long get_makespan(struct schedule_t * S);

#endif // _SCHEDULE_H_
