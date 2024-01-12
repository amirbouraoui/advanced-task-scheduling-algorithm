#ifndef _LIST_H_
#define _LIST_H_

/**
 * @brief
 * Le fichier définit une liste générique homogène doublement chaînée.
 */

/**
 * @brief Un noeud d'une liste doublement chaînée contient
 * (+) une donnée (data),
 * (+) une référence (successor) à l'élément suivant, et
 * (+) une référence (predecessor) à l'élément précédent.
 */
struct list_node_t {
	void * data;
	struct list_node_t * successor, * predecessor;
};

/**
 * @brief Restitue la donnée du nœud \p node.
 * 
 * @param[in] node 
 * @return void* 
 */
void * get_list_node_data(const struct list_node_t * node);

/**
 * @brief Restitue le successeur du nœud \p node.
 * 
 * @param[in] node 
 * @return list_node_t* 
 */
struct list_node_t * get_successor(const struct list_node_t * node);

/**
 * @brief Restitue le prédécesseur du nœud \p node.
 * 
 * @param[in] node 
 * @return list_node_t* 
 */
struct list_node_t * get_predecessor(const struct list_node_t * node);

/**
 * @brief Remplace la donnée du nœud \p node par \p newData.
 * 
 * @param[in] node 
 * @param[in] newData 
 */
void set_list_node_data(struct list_node_t * node, void * newData);

/**
 * @brief Remplace le successeur du nœud \p node par \p newSucc.
 * 
 * @param[in] node 
 * @param[in] newSucc 
 */
void set_successor(struct list_node_t * node, struct list_node_t * newSucc);

/**
 * @brief Remplace le prédécesseur du nœud \p node par \p newSucc.
 * 
 * @param[in] node 
 * @param[in] NewPred 
 */
void set_predecessor(struct list_node_t * node, struct list_node_t * NewPred);

/**
 * @brief La liste est une structure contenant :
 * (+) une référence (head) sur son premier élément,
 * (+) une référence (tail) sur son dernier élément,
 * (+) le nombre d'éléments,
 * (+) un pointeur de fonction pour afficher ses données, et
 * (+) un pointeur de fonction pour libérer la mémoire de ses données.
 */
struct list_t {
	struct list_node_t * head, * tail;
	int numelm;
	void (*viewData)(const void * data);
	void (*freeData)(void * data);
};

/**
 * @brief Construit et initialise une liste vide.
 * 
 * Les deux pointeurs de fonction passés en paramètre indiquent
 * de façon implicite le type de la donnée de la liste créée.
 * 
 * @param[in] viewData Pointeur de fonction pour afficher les données de la nouvelle liste.
 * @param[in] freeData Pointeur de fonction pour libérer la mémoire des données de la nouvelle liste.
 * @return list_t* La nouvelle liste créée.
 */
struct list_t * new_list(void (*viewData)(const void *), void (*freeData)(void *));

/**
 * @brief Renvoie 1 si la liste \p L est vide, sinon renvoie 0.
 * 
 * @param[in] L 
 * @return int 
 */
int list_is_empty(struct list_t * L);

/**
 * @brief Restitue la taille (nombre d'éléments) de la liste \p L.
 * 
 * @param[in] L 
 * @return int 
 */
int get_list_size(const struct list_t * L);

/**
 * @brief Restitue la tête (premier élément) de la liste \p L.
 * 
 * @param[in] L 
 * @return list_node_t* 
 */
struct list_node_t * get_list_head(const struct list_t * L);

/**
 * @brief Restitue la queue (dernier élément) de la liste \p L.
 * 
 * @param[in] L 
 * @return list_node_t* 
 */
struct list_node_t * get_list_tail(const struct list_t * L);

/**
 * @brief Incrémente la taille de la liste \p L par 1.
 * 
 * @param[in] L 
 */
void increase_list_size(struct list_t * L);

/**
 * @brief Décrémente la taille de la liste \p L par 1.
 * 
 * @param[in] L 
 */
void decrease_list_size(struct list_t * L);

/**
 * @brief Met à jour la taille de la liste \p L à \p newSize.
 * 
 * @param[in] L 
 * @param[in] newSize 
 */
void set_list_size(struct list_t * L, int newSize);

/**
 * @brief Remplace la tête de la liste \p L par \p newHead.
 * 
 * @param[in] L 
 * @param[in] newHead 
 */
void set_head(struct list_t * L, struct list_node_t * newHead);

/**
 * @brief Remplace la queue de la liste \p L par \p newTail.
 * 
 * @param[in] L 
 * @param[in] newTail 
 */
void set_tail(struct list_t * L, struct list_node_t * newTail);

/**
 * @brief Deux possibilités pour liberer la mémoire de la liste \p L :
 * (+) Si le paramètre \p deleteData vaut 0,
 *     alors les données (data) référencées par les éléments
 *     de la liste \p L ne sont pas supprimées ;
 * (+) Si le paramètre \p deleteData vaut 1,
 *     alors le pointeur de fonction freeData de la structure list_t
 *     va servir à supprimer les données (data) référencées par
 *     les éléments de la liste \p L.
 * 
 * @param[in] L 
 * @param[in] deleteData 
 */
void delete_list(struct list_t * L, int deleteData);

/**
 * @brief Affiche les éléments de la liste \p L.
 * Les données de chaque élément sont affichées grâce au pointeur
 * de fonction viewData de la structure list_t.
 * 
 * @param[in] L 
 */
void view_list(const struct list_t * L);

/**
 * @brief Insère en tête de la liste \p L un nouveau nœud de donnée \p data.
 * 
 * @param[in] L 
 * @param[in] data
 */
void list_insert_first(struct list_t * L, void * data);

/**
 * @brief Insère à la fin de la liste \p L un nouveau nœud de donnée \p data.
 * 
 * @param[in] L 
 * @param[in] data
 */
void list_insert_last(struct list_t * L, void * data);

/**
 * @brief Insère un nouveau nœud de donnée \p data dans la liste \p L
 * après le nœud indiqué par le pointeur \p ptrelm.
 * 
 * @param[in] L
 * @param[in] data
 * @param[in] ptrelm Ce pointeur désigne obligatoirement
 * 					un nœud de la liste \p L.
 */
void list_insert_after(struct list_t * L, void * data, struct list_node_t * ptrelm);

/**
 * @brief Supprime le premier nœud de la liste \p L et restitue sa donnée.
 * La mémoire du nœud supprimé est libérée mais pas la mémoire de la donnée.
 * 
 * @param[in] L La liste ne doit pas être vide.
 * @return void* La donnée du nœud supprimé.
 */
void * list_remove_first(struct list_t * L);

/**
 * @brief Supprime le dernier nœud de la liste \p L et restitue sa donnée.
 * La mémoire du nœud supprimé est libérée mais pas la mémoire de la donnée.
 * 
 * @param[in] L La liste ne doit pas être vide.
 * @return void* La donnée du nœud supprimé.
 */
void * list_remove_last(struct list_t * L);

/**
 * @brief Supprime le nœud de la liste \p L indiqué par le pointeur \p node
 * et restitue sa donnée.
 * La mémoire du nœud supprimé est libérée mais pas la mémoire de la donnée.
 * 
 * @param[in] L La liste ne doit pas être vide.
 * @param[in] node Ce pointeur désigne obligatoirement un nœud de la liste \p L.
 * @return void* La donnée du nœud supprimé.
 */
void * list_remove_node(struct list_t * L, struct list_node_t * node);

#endif // _LIST_H_