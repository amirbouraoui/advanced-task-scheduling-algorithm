#ifndef _INSTANCE_H_
#define _INSTANCE_H_

/**
 * @brief
 * Le fichier définit une instance d'un problème d'ordonnancement.
 * Avant cela, il y a la définition de la notion d'une tâche.
 */

/**
 * @brief
 * Une tâche est caractérisée par les attributs suivants :
 * (+) un identifiant (id),
 * (+) sa durée (processing_time), et
 * (+) sa date de libération (release_time),
 *     c'est-à-dire le moment à partir duquel on peut commencer à l'exécuter
 *     (ne pas confondre avec la date de début de son exécution;
 *     cela est une caractéristique de l'ordonnancement et pas de l'instance).
 */
struct task_t {
	char * id;
	unsigned long processing_time;
	unsigned long release_time;
};

/**
 * @brief
 * Construire et initialiser une nouvelle tâche.
 *
 * @param[in] id L'identifiant de la nouvelle tâche.
 * @param[in] proctime Le temps d’exécution de la nouvelle tâche.
 * @param[in] reltime La date de libération de la nouvelle tâche.
 * @return struct task_t*
 */
struct task_t * new_task(char * id, unsigned long proctime, unsigned long reltime);

/**
 * @brief Restitue l'identifiant de la tâche \p task.
 * 
 * @param[in] task 
 * @return char* 
 */
char * get_task_id(const struct task_t * task);

/**
 * @brief Restitue le temps d’exécution de la tâche \p task.
 * 
 * @param[in] task 
 * @return unsigned long 
 */
unsigned long get_task_processing_time(const struct task_t * task);

/**
 * @brief Restitue la date de libération de la tâche \p task.
 * 
 * @param[in] task 
 * @return unsigned long 
 */
unsigned long get_task_release_time(const struct task_t * task);

/**
 * @brief Afficher les caractéristiques de la tâche \p task.
 * 
 * @param[in] task 
 */
void view_task(const void * task);

/**
 * @brief Supprimer et libérer la mémoire de la tâche \p task.
 * 
 * @param[in] task 
 */
void delete_task(void * task);

/**
 * @brief
 * Une instance est une liste doublement chaînée des tâches.
 */
typedef struct list_t * Instance;

/**
 * @brief
 * Construire une nouvelle instance avec le contenu du fichier \p filename.
 * 
 * @param[in] filename 
 * @return Instance 
 */
Instance read_instance(const char * filename);

/**
 * @brief
 * Afficher les tâches de l'instance \p I.
 * 
 * @param[in] I 
 */
void view_instance(Instance I);

/**
 * @brief
 * Liberer la mémoire d'une instance \p I.
 * Si le paramètre \p deleteData vaut 1,
 * les données correspondant aux tâches de l'instance sont aussi supprimer.
 * 
 * @param[in] I 
 * @param[in] deleteData 
 */
void delete_instance(Instance I, int deleteData);

#endif // _INSTANCE_H_