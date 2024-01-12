#ifndef _TREE_H_
#define _TREE_H_

/**
 * @brief
 * Le fichier définit un arbre binaire de recherche générique équilibré ou non-équilibré.
 */

/**
 * @brief
 * Un élément d'un arbre binaire de recherche contient
 * (+) une clé (key),
 * (+) une donnée (data),
 * (+) le facteur d'équilibre (bfactor) :
 *     différence entre la hauteur du fils gauche et la hauteur du fils droit
 *     NB : le facteur d'équilibre d'une feuille est égal à 0,
 * (+) la référence left vers le fils gauche, et
 * (+) la référence right vers le fils droit.
 */
struct tree_node_t {
	void * key;
	void * data;
	int bfactor;
	struct tree_node_t * left;
	struct tree_node_t * right;
};

/**
 * @brief Renvoie 1 si le nœud \p node est vide, sinon renvoie 0.
 * 
 * @param[in] node 
 * @return int 
 */
int tree_node_is_empty(struct tree_node_t * node);

/**
 * @brief Restitue la clé du nœud \p node.
 * 
 * @param[in] node 
 * @return void* 
 */
void * get_tree_node_key(const struct tree_node_t * node);

/**
 * @brief Restitue la donnée du nœud \p node.
 * 
 * @param[in] node 
 * @return void* 
 */
void * get_tree_node_data(const struct tree_node_t * node);

/**
 * @brief Restitue le fils gauche du nœud \p node.
 * 
 * @param[in] node 
 * @return tree_node_t* 
 */
struct tree_node_t * get_left(const struct tree_node_t * node);

/**
 * @brief Restitue le fils droit du nœud \p node.
 * 
 * @param[in] node 
 * @return tree_node_t* 
 */
struct tree_node_t * get_right(const struct tree_node_t * node);

/**
 * @brief Restitue le facteur d'équilibre du nœud \p node.
 * 
 * @param[in] node 
 * @return int 
 */
int get_bfactor(const struct tree_node_t * node);

/**
 * @brief Remplace la clé du nœud \p node par \p newKey.
 * 
 * @param[in] node 
 * @param[in] newKey 
 */
void set_tree_node_key(struct tree_node_t * node, void * newKey);

/**
 * @brief Remplace la donnée du nœud \p node par \p newData.
 * 
 * @param[in] node 
 * @param[in] newData 
 */
void set_tree_node_data(struct tree_node_t * node, void * newData);

/**
 * @brief Remplace le fils gauche du nœud \p node par \p newLeft.
 * 
 * @param[in] node 
 * @param[in] newLeft 
 */
void set_left(struct tree_node_t * node, struct tree_node_t * newLeft);

/**
 * @brief Remplace le fils droit du nœud \p node par \p newRight.
 * 
 * @param[in] node 
 * @param[in] newRight 
 */
void set_right(struct tree_node_t * node, struct tree_node_t * newRight);

/**
 * @brief Incrémente le facteur d'équilibre du nœud \p node par 1.
 * 
 * @param[in] node 
 * @param[in] newBFactor 
 */
void increase_bfactor(struct tree_node_t * node);

/**
 * @brief Décrémente le facteur d'équilibre du nœud \p node par 1.
 * 
 * @param[in] node 
 * @param[in] newBFactor 
 */
void decrease_bfactor(struct tree_node_t * node);

/**
 * @brief Remplace le facteur d'équilibre du nœud \p node par \p newBFactor.
 * 
 * @param[in] node 
 * @param[in] newBFactor 
 */
void set_bfactor(struct tree_node_t * node, int newBFactor);

/**
 * L'arbre binaire de recherche est une structure contenant :
 * (+) une référence (root) sur sa racine,
 * (+) le nombre d'éléments,
 * (+) un pointeur de fonction pour comparer ses clés,
 * (+) un pointeur de fonction pour afficher ses clés,
 * (+) un pointeur de fonction pour afficher ses données,
 * (+) un pointeur de fonction pour libérer la mémoire de ses clés,
 * (+) un pointeur de fonction pour libérer la mémoire de ses données, et
 * (+) une indication (balanced) si l'arbre binaire de recherche
 *     est équilibré (balanced=1) ou pas (balanced=0).
 */
struct tree_t {
	struct tree_node_t * root;
	int numelm;
	int (*preceed)(const void * a, const void * b);
	void (*viewKey)(const void * key);
	void (*viewData)(const void * data);
	void (*freeKey)(void * key);
	void (*freeData)(void * data);
	int balanced;
};

/**
 * @brief Construire un arbre binaire de recherche vide.
 * 
 * @param balanced Indique si l'arbre doit être équilibré (valeur 1) ou pas (valeur 0).
 * @param preceed Pointeur de fonction pour comparer deux clés de l'arbre.
 * @param viewKey Pointeur de fonction pour afficher la clé d'un nœud de l'arbre.
 * @param viewData Pointeur de fonction pour afficher la donnée d'un nœud de l'arbre.
 * @param freeKey Pointeur de fonction pour libérer la mémoire de la clé d'un nœud de l'arbre.
 * @param freeData Pointeur de fonction pour libérer la mémoire de la donnée d'un nœud de l'arbre.
 * @return struct tree_t* 
 */
struct tree_t * new_tree(int balanced, int (*preceed)(const void *, const void *),
						void (*viewKey)(const void *), void (*viewData)(const void *),
						void (*freeKey)(void *), void (*freeData)(void *));

/**
 * @brief Renvoie 1 si l'arbre \p T est vide, sinon renvoie 0.
 * 
 * @param[in] T 
 * @return int 
 */
int tree_is_empty(struct tree_t * T);

/**
 * @brief Renvoie 1 si l'arbre \p T est équilibré, sinon renvoie 0.
 * 
 * @param[in] T 
 * @return int 
 */
int tree_is_balanced(struct tree_t * T);

/**
 * @brief Restitue la taille (nombre d'éléments) de l'arbre \p T.
 * 
 * @param[in] T 
 * @return int 
 */
int get_tree_size(const struct tree_t * T);

/**
 * @brief Restitue la racine de l'arbre \p T.
 * 
 * @param[in] T 
 * @return tree_node_t* 
 */
struct tree_node_t * get_root(const struct tree_t * T);

/**
 * @brief Incrémente la taille de l'arbre \p T par 1.
 * 
 * @param[in] T 
 */
void increase_tree_size(struct tree_t * T);

/**
 * @brief Décrémente la taille de l'arbre \p T par 1.
 * 
 * @param[in] T 
 */
void decrease_tree_size(struct tree_t * T);

/**
 * @brief Remplace la racine de l'arbre \p T par \p newRoot.
 * 
 * @param[in] T 
 * @param[in] newRoot 
 */
void set_root(struct tree_t * T, struct tree_node_t * newRoot);

/**
 * @brief
 * Plusieurs possibilités de supprimer l'arbre binaire de recherche T :
 * (+) Si le paramètre deleteKey vaut 0
 *     Alors les clés (key) des éléments de l'arbre T ne sont pas supprimées ;
 * (+) Si le paramètre deleteKey vaut 1
 *     Alors le pointeur de fonction freeKey de la structure BinarySearchTree
 *     va servir à supprimer les clés (key) des éléments de l'arbre T.
 *
 * (+) Si le paramètre deleteData vaut 0
 *     Alors les données (data) référencées par les éléments
 *     de l'arbre T ne sont pas supprimées ;
 * (+) Si le paramètre deleteData vaut 1
 *     Alors le pointeur de fonction freeData de la structure BinarySearchTree
 *     va servir à supprimer les données (data) référencées par
 *     les éléments de l'arbre T.
 * 
 * @param[in] T 
 * @param[in] deleteKey 
 * @param[in] deleteData 
 */
void delete_tree(struct tree_t * T, int deleteKey, int deleteData);

/**
 * @brief
 * Afficher les éléments de l'arbre binaire de recherche T.
 * Chaque élément est affiché grâce aux pointeurs de fonction
 * de la structure BinarySearchTree:
 * (+) viewKey pour la clé
 * (+) viewData pour les données
 * L'arbre est affiché dans l'ordre infixe.
 * 
 * @param[in] T 
 */
void view_tree(const struct tree_t * T);

/**
 * @brief
 * Ajouter dans l'arbre binaire de recherche \p T un élément de clé \p key et de donnée \p data.
 * 
 * @param[in] T 
 * @param[in] key 
 * @param[in] data 
 */
void tree_insert(struct tree_t * T, void * key, void * data);

/**
 * @brief
 * Trouver et renvoyer le nœud de clé minimum du sous-arbre raciné au nœud \p curr.
 * NB : fonction récursive.
 * 
 * @param[in] curr 
 * @return struct tree_node_t* 
 */
struct tree_node_t * tree_min(struct tree_node_t * curr);

/**
 * @brief
 * Trouver et renvoyer le nœud de clé maximum du sous-arbre raciné au nœud \p curr.
 * NB : fonction récursive.
 * 
 * @param[in] curr 
 * @return struct tree_node_t* 
 */
struct tree_node_t * tree_max(struct tree_node_t * curr);

/**
 * @brief 
 * Chercher dans le sous-arbre raciné au nœud \p curr et renvoyer le nœud avec clé \p key.
 * Le pointeur de fonction \p preceed donne l'ordre entre deux clés.
 * NB1 : fonction récursive.
 * NB2 : on fait l'hypothèse que la clé \p key existe dans le sous-arbre raciné au nœud \p curr.
 * 
 * @param[in] curr 
 * @param[in] key 
 * @param[in] preceed 
 * @return struct tree_node_t* 
 */
struct tree_node_t * tree_find_node(struct tree_node_t * curr, void * key, int (*preceed)(const void *, const void *));

/**
 * @brief
 * Chercher dans le sous-arbre raciné au nœud \p curr et renvoyer le prédécesseur du nœud avec clé \p key.
 * Le prédécesseur est le nœud qui contient la clé la plus grande qui est plus petite que la clé \p key.
 * Le pointeur de fonction \p preceed donne l'ordre entre deux clés.
 * NB1 : fonction récursive.
 * NB2 : on fait l'hypothèse que la clé \p key existe dans le sous-arbre raciné au nœud \p curr.
 * 
 * @param[in] curr 
 * @param[in] key 
 * @param[in] preceed 
 * @return struct tree_node_t* 
 */
struct tree_node_t * tree_find_predecessor(struct tree_node_t * curr, void * key, int (*preceed)(const void *, const void *));

/**
 * @brief 
 * Chercher dans le sous-arbre raciné au nœud \p curr et renvoyer le successeur du nœud avec clé \p key.
 * Le successeur est le nœud qui contient la clé la plus petite qui est plus grande que la clé \p key.
 * Le pointeur de fonction \p preceed donne l'ordre entre deux clés.
 * NB1 : fonction récursive.
 * NB2 : on fait l'hypothèse que la clé \p key existe dans le sous-arbre raciné au nœud \p curr.
 * 
 * @param[in] curr 
 * @param[in] key 
 * @param[in] preceed 
 * @return struct tree_node_t* 
 */
struct tree_node_t * tree_find_successor(struct tree_node_t * curr, void * key, int (*preceed)(const void *, const void *));

/**
 * @brief 
 * Supprime le nœud avec clé \p key de l'arbre \p T et restitue sa donnée.
 * La clé \p key existe obligatoirement dans l'arbre \p T.
 * La mémoire du nœud supprimé ainsi que de sa clé est libérée mais pas la mémoire de la donnée.
 * 
 * @param[in] T 
 * @param[in] key 
 * @return void* 
 */
void * tree_remove(struct tree_t * T, void * key);

#endif // _TREE_H_