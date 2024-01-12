#ifndef _ALGO_H_
#define _ALGO_H_

#include "instance.h"

/**
 * @brief Créer et restituer un ordonnancement sur \p m machines en donnant comme entrée l'instance \p I.
 * Le paramètre \p preemption vaut 1 si les préemptions sont autorisées, sinon ça vaut 0.
 * Le paramètre \p balanced_tree vaut 1 si les arbres binaires de recherche utilisé dans la structure d'événements
 * et dans la file d'attente de "ready tasks" sont équilibrés, sinon ça vaut 0.
 * 
 * @param[in] I 
 * @param[in] num_m 
 * @param[in] preemption 
 * @return struct schedule_t* 
 */
struct schedule_t * create_schedule(Instance I, int num_m, int preemption, int balanced_tree);

#endif // _ALGO_H_
