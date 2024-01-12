#include "tree.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "list.h"
#include "util.h"

/********************************************************************
 * tree_node_t
 ********************************************************************/

/**
 * @brief
 * Construire et initialiser un nouveau nœud d'un arbre binaire de recherche.
 * Le facteur d'équilibre est initialisé à zero.
 * 
 * @param[in] key Clé du nouveau nœud.
 * @param[in] data Donnée à affecter au nouveau nœud.
 * @return struct tree_node_t* Le nouveau nœud créé.
 */
static struct tree_node_t * new_tree_node(void * key, void * data) {
	// create new tree node
	struct tree_node_t * node = (struct tree_node_t *)malloc(sizeof(struct tree_node_t));

	if (node == NULL) 
		ShowMessage("Erreur d'allocation mémoire! new_tree_node()", 1);

	// initialize node
	node->key = key;
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	node->bfactor = 0;

	return node;
}

int tree_node_is_empty(struct tree_node_t * node) {
	return (node == NULL);
}

void * get_tree_node_key(const struct tree_node_t * node) {
	return node->key;
}

void * get_tree_node_data(const struct tree_node_t * node) {
	return node->data;
}

struct tree_node_t * get_left(const struct tree_node_t * node) {
	return node->left;
}

struct tree_node_t * get_right(const struct tree_node_t * node) {
	return node->right;
}

int get_bfactor(const struct tree_node_t * node) {
	return node->bfactor;
}

void set_tree_node_key(struct tree_node_t * node, void * newKey) {
	node->key = newKey;
}

void set_tree_node_data(struct tree_node_t * node, void * newData) {
	node->data = newData;
}

void set_left(struct tree_node_t * node, struct tree_node_t * newLeft) {
	node->left = newLeft;
}

void set_right(struct tree_node_t * node, struct tree_node_t * newRight) {
	node->right = newRight;
}

void increase_bfactor(struct tree_node_t * node) {
	node->bfactor++;
}

void decrease_bfactor(struct tree_node_t * node) {
	node->bfactor--;
}

void set_bfactor(struct tree_node_t * node, int newBFactor) {
	node->bfactor = newBFactor;
}

/*********************************************************************
 * tree_t
 *********************************************************************/

struct tree_t * new_tree(int balanced, int (*preceed)(const void *, const void *),
							void (*viewKey)(const void *), void (*viewData)(const void *),
							void (*freeKey)(void *), void (*freeData)(void *)) {
	// Construire un arbre binaire de recherche vide.
	struct tree_t * tree = (struct tree_t *)malloc(sizeof(struct tree_t));

	if (tree == NULL)
		ShowMessage("Erreur d'allocation mémoire (new_tree)", 1);

	// Initialize tree
	tree->root = NULL;
	tree->numelm = 0;
	tree->balanced = balanced;
	tree->preceed = preceed;
	tree->viewKey = viewKey;
	tree->viewData = viewData;
	tree->freeKey = freeKey;
	tree->freeData = freeData;

	return tree;
}

int tree_is_empty(struct tree_t * T) {
	return (T->root == NULL);
}

int tree_is_balanced(struct tree_t * T) {
	return T->balanced;
}

int get_tree_size(const struct tree_t * T) {
	return T->numelm;
}

struct tree_node_t * get_root(const struct tree_t * T) {
	return T->root;
}

void increase_tree_size(struct tree_t * T) {
	T->numelm++;
}

void decrease_tree_size(struct tree_t * T) {
	T->numelm--;
}

void set_root(struct tree_t * T, struct tree_node_t * newRoot) {
	T->root = newRoot;
}

/**
 * @brief
 * Libérer récursivement le sous-arbre raciné au nœud curr.
 * Dans le cas où le pointeur de fonction freeKey (resp. freeData) n'est pas NULL,
 * la mémoire de la clé (resp. de la donnée) du nœud actuel est aussi libérée.
 * NB : procédure récursive.
 * 
 * @param[in] curr 
 * @param[in] freeKey 
 * @param[in] freeData 
 */
static void delete_tree_node(struct tree_node_t * curr, void (*freeKey)(void *), void (*freeData)(void *)) {
	if (curr == NULL)
		return;

	// Recursively deleting the left and right subtrees
	delete_tree_node(curr->left, freeKey, freeData);
	delete_tree_node(curr->right, freeKey, freeData);

	// If freeKey is not NULL, free the memory of the key of the current node
	if (freeKey != NULL)
		freeKey(curr->key);

	// Same for the data
	if (freeData != NULL)
		freeData(curr->data);

	// Free the memory of the current node
	free(curr);
}

/**
 * NB : Utiliser la procédure récursive delete_tree_node.
 * Vous devez utiliser les bons paramètres freeKey et freeData
 * par rapport aux valeurs deleteKey et deleteData.
 */
void delete_tree(struct tree_t * T, int deleteKey, int deleteData) {
	assert(deleteKey == 0 || deleteKey == 1);
	assert(deleteData == 0 || deleteData == 1);

	// If deleteKey is non-zero, freeKey is set to T->freeKey.
	// If deleteKey is zero, freeKey is set to NULL.
	void (*freeKey)(void *) = (deleteKey) ? T->freeKey : NULL;

	// If deleteData is non-zero, freeData is set to T->freeData.
	// If deleteData is zero, freeData is set to NULL.
	void (*freeData)(void *) = (deleteData) ? T->freeData : NULL;

	// Delete the tree recursively
	delete_tree_node(T->root, freeKey, freeData);

	// Reset the values of the tree
	T->root = NULL;
	T->numelm = 0;
	T->balanced = 0;
}

/**
 * @brief
 * Afficher récursivement le sous-arbre raciné au nœud curr
 * en utilisant un ordre infixe.
 * NB : procédure récursive.
 *
 * @param[in] curr 
 * @param[in] viewKey 
 * @param[in] viewData 
 */
static void view_tree_inorder(struct tree_node_t * curr,
							  void (*viewKey)(const void *),
							  void (*viewData)(const void *)) {
	// Verifier si l'arbre est vide
	if (tree_node_is_empty(curr)){
		return;
	}

	// Recursion infixé pour afficher les clés et les données
	view_tree_inorder(curr->left, viewKey, viewData);
	viewKey(curr->key);
	//viewData(curr->data);	// Don't need this in Debug mode
	view_tree_inorder(curr->right, viewKey, viewData);
}

/**
 * NB : Utiliser la procédure récursive view_tree_inorder.
 */
void view_tree(const struct tree_t * T) {
	assert(T != NULL);
	view_tree_inorder(T->root, T->viewKey, T->viewData);
}

/**
 * @brief
 * Effectuer une rotation gauche autour du nœud \p y.
 * N'oubliez pas à mettre à jour les facteurs d'équilibre (bfactor) des nœuds
 * modifiés. Pour ce dernier, il y a 5 cas à considérer :
 * (+) bfactor(y)=-2 et bfactor(y->right)=-1
 * (+) bfactor(y)=-2 et bfactor(y->right)=0
 * (+) bfactor(y)=-1 et bfactor(y->right)=1
 * (+) bfactor(y)=-1 et bfactor(y->right)=-1
 * (+) bfactor(y)=-1 et bfactor(y->right)=0
 * (+) bfactor(y)=-2 et bfactor(y->right)=-2
 * 
 * @param[in] y 
 * @return struct tree_node_t* 
 */
static struct tree_node_t *rotate_left(struct tree_node_t *y)
{
	assert(y);
	assert(y->right);

	struct tree_node_t *x = y->right;
	struct tree_node_t *T2 = x->left;

	// Perform the left rotation
	x->left = y;
	y->right = T2;

	// Update the balance factors
	if (x->bfactor == -1)
	{
		// Case (+) bfactor(y)=-2 and bfactor(y->right)=-1
		y->bfactor = 0;
		x->bfactor = 0;
	}
	else if (x->bfactor == 0)
	{
		// Case (+) bfactor(y)=-2 and bfactor(y->right)=0
		y->bfactor = 1;
		x->bfactor = -1;
	}
	else if (x->bfactor == 1)
	{
		if (T2 == NULL || ((struct tree_node_t *)T2)->bfactor == 0)
		{
			// Case (+) bfactor(y)=-1 and bfactor(y->right)=0
			y->bfactor = -1;
			x->bfactor = 1;
		}
		else if (((struct tree_node_t *)T2)->bfactor == -1)
		{
			// Case (+) bfactor(y)=-1 and bfactor(y->right)=-1
			y->bfactor = 0;
			x->bfactor = 1;
		}
		else if (((struct tree_node_t *)T2)->bfactor == 1)
		{
			// Case (+) bfactor(y)=-1 and bfactor(y->right)=1
			y->bfactor = -1;
			x->bfactor = 0;
		}
	}
	else if (x->bfactor == 2)
	{
		// Case (+) bfactor(y)=-2 and bfactor(y->right)=2 (bizzare ne devrait pas arriver)
		y->bfactor = 1;
		x->bfactor = 1;
	}

	return x;
}

/**
 * @brief
 * Effectuer une rotation droite autour du nœud \p x.
 * N'oubliez pas à mettre à jour les facteurs d'équilibre (bfactor) des nœuds
 * modifiés. Pour ce dernier, il y a 5 cas à considérer :
 * (+) bfactor(x)=2 et bfactor(x->left)=1
 * (+) bfactor(x)=2 et bfactor(x->left)=0
 * (+) bfactor(x)=1 et bfactor(x->left)=1
 * (+) bfactor(x)=1 et bfactor(x->left)=-1
 * (+) bfactor(x)=1 et bfactor(x->left)=0
 * (+) bfactor(x)=2 et bfactor(x->left)=2
 * 
 * @param[in] x 
 * @return struct tree_node_t* 
 */
static struct tree_node_t *rotate_right(struct tree_node_t *x)
{
	assert(x);
	assert(x->left);

	struct tree_node_t *y = x->left;
	struct tree_node_t *T2 = y->right;

	// Perform the right rotation
	y->right = x;
	x->left = T2;

	// Update the balance factors
	if (y->bfactor == 1)
	{
		// Case (+) bfactor(x)=2 and bfactor(x->left)=1
		x->bfactor = 0;
		y->bfactor = 0;
	}
	else if (y->bfactor == 0)
	{
		// Case (+) bfactor(x)=2 and bfactor(x->left)=0
		x->bfactor = 1;
		y->bfactor = -1;
	}
	else if (y->bfactor == -1)
	{
		if (T2 == NULL || ((struct tree_node_t *)T2)->bfactor == 0)
		{
			// Case (+) bfactor(x)=1 and bfactor(x->left)=0
			x->bfactor = 1;
			y->bfactor = -1;
		}
		else if (((struct tree_node_t *)T2)->bfactor == -1)
		{
			// Case (+) bfactor(x)=1 and bfactor(x->left)=-1
			x->bfactor = 0;
			y->bfactor = 0;
		}
		else if (((struct tree_node_t *)T2)->bfactor == 1)
		{
			// Case (+) bfactor(x)=1 and bfactor(x->left)=1
			x->bfactor = 1;
			y->bfactor = 0;
		}
	}
	else if (y->bfactor == 2)
	{
		// Case (+) bfactor(x)=2 and bfactor(x->left)=2 (bizzare ne devrait pas arriver)
		x->bfactor = 1;
		y->bfactor = 1;
	}

	return y;
}

/**
 * @brief
 * Insérer un nouveau nœud de clé \p key et donnée \p data
 * au sous-arbre binaire de recherche raciné au nœud \p curr.
 * Les clés sont comparées en utilisant le pointeur de fonction \p preceed.
 * N'oubliez pas à mettre à jour la facteur d'équilibre du nœud \p curr.
 * N'oubliez pas à faire les rotations nécessaires (4 cas à considérer)
 * si le paramètre \p balanced indique que l'arbre est équilibré.
 * La fonction renvoie le nœud curant éventuellement mis à jour.
 * NB : fonction récursive.
 * 
 * @param[in] curr 
 * @param[in] key 
 * @param[in] data 
 * @param[in] balanced 
 * @param[in] preceed 
 * @return struct tree_node_t* 
 */
static struct tree_node_t * insert_into_tree_node(struct tree_node_t * curr, void * key, void * data,
											int balanced, int (*preceed)(const void *, const void *)) {
	// PARTIE 1 :
	// Mettez ici le code de l'insertion:
	// - exception
	// - recherche récursif de la position à insérer
	// - mise à jour du facteur d'équilibre

	// Cas noeud vide,cree un nouveau, exception
	if (tree_node_is_empty(curr)){
		return new_tree_node(key, data);
	}

	// Avec l'entete et l'information de la ligne 288 on compare la clé rentrer
	int Resu = preceed(key, curr->key);
	if ( Resu < 0 ) // Corriger c'est bon c'est dans le bon sens
	{
		curr->left = insert_into_tree_node(curr->left, key, data, balanced, preceed);
	}
	else
	{
		curr->right = insert_into_tree_node(curr->right, key, data, balanced, preceed);
	}

	if ( balanced ) {
		// PARTIE 2 :
		// Gérer ici les rotations
		// a partir info du brief ligne 243 a 273
		int FacteurBalance = get_bfactor(curr);

		if (FacteurBalance > 1) {
			 if (get_bfactor(curr->left) < 0){
				curr->left = rotate_left(curr->left);
			}
			curr = rotate_right(curr);

		}else if (FacteurBalance < -1) {
			if (get_bfactor(curr->right) > 0)
			{
				curr->right = rotate_right(curr->right);
			}
			curr = rotate_left(curr);
		}
	}

	return curr;
}

/**
 * NB : Utiliser la fonction récursive insert_into_tree_node.
 */
void tree_insert(struct tree_t * T, void * key, void * data) {
	assert(T != NULL && T->preceed != NULL);
	T->root = insert_into_tree_node(T->root, key, data, T->balanced, T->preceed);
}

struct tree_node_t * tree_min(struct tree_node_t * curr) {
	assert(!tree_node_is_empty(curr));

	if (tree_node_is_empty(curr->left))
		return curr;
	else
		return tree_min(curr->left);
}

struct tree_node_t * tree_max(struct tree_node_t * curr) {
	assert(!tree_node_is_empty(curr));

	if (tree_node_is_empty(curr->right))
		return curr;
	else
		return tree_max(curr->right);
}

struct tree_node_t * tree_find_node(struct tree_node_t * curr, void * key, int (*preceed)(const void *, const void *)) {
	assert(!tree_node_is_empty(curr) && key != NULL);

	int cmp = preceed(key, get_tree_node_key(curr));

	if (cmp == 0){
		return curr;
	}else if (cmp < 0){
		return tree_find_node(get_left(curr), key, preceed);
	}else{
		return tree_find_node(get_right(curr), key, preceed);
	}
}

struct tree_node_t * tree_find_predecessor(struct tree_node_t * curr, void * key, int (*preceed)(const void *, const void *)) {
	assert(!tree_node_is_empty(curr) && key != NULL);

	int cmp = preceed(curr->key, key);
	if (cmp >= 0)
	{
		if (!tree_node_is_empty(curr->left)){
			return tree_max(curr->left);
		}
	}
	else
	{
		if (!tree_node_is_empty(curr->left))
		{
			struct tree_node_t *pred = tree_find_predecessor(curr->left, key, preceed);
			if (!tree_node_is_empty(pred)){
				return pred;
			}
		}
		return curr;
	}
	return NULL;

}

struct tree_node_t * tree_find_successor(struct tree_node_t * curr, void * key, int (*preceed)(const void *, const void *)) {
	assert(!tree_node_is_empty(curr) && key != NULL);

	struct tree_node_t *node = curr;

	if (!tree_node_is_empty(node->right)){
		return tree_min(node->right);
	}

	// If the right child of the current node is empty,
	// the successor is the lowest ancestor of the current node whose left child is also an ancestor of the current node.
	struct tree_node_t *successor = NULL;

	while (node != NULL){

		if (preceed(key, node->key) == -1){
			successor = node;
			node = node->left;

		}else{
			node = node->right;
		}
	}

	return successor;
}

/**
 * @brief 
 * Supprimer le nœud de clé \p key du sous-arbre binaire de recherche raciné au nœud \p curr.
 * La donnée du nœud supprimé est restituée en utilisant le pointeur \p data (passage de paramètre par référence).
 * Les clés sont comparées en utilisant le pointeur de fonction \p preceed.
 * N'oubliez pas à mettre à jour la facteur d'équilibre du nœud \p curr.
 * N'oubliez pas à faire les rotations nécessaires (6 cas à considérer)
 * si le paramètre \p balanced indique que l'arbre est équilibré.
 * La fonction renvoie le nœud curant éventuellement mis à jour.
 * NB : fonction récursive.
 * 
 * @param[in] curr 
 * @param[in] key 
 * @param[out] data 
 * @param[in] balanced 
 * @param[in] preceed 
 * @return struct tree_node_t* 
 */
static struct tree_node_t * remove_tree_node(struct tree_node_t * curr, void * key, void ** data, int balanced,
											int (*preceed)(const void *, const void *)) {
	assert(curr);
	// PARTIE 1
	int cmp = preceed(key, curr->key);	// event_preceed()

	if (cmp == 0){
		*data = curr->data;
		if (curr->left && curr->right) {
			struct tree_node_t *successor = tree_find_successor(curr, key, preceed);
			curr->key = successor->key;
			curr->data = successor->data;
			curr->right = remove_tree_node(curr->right, successor->key, data, 1, preceed);
		}else{
			struct tree_node_t *temp = NULL;
			if (!curr->left)
				temp = curr->right;
			
			if (!curr->right)
				temp = curr->left;

			free(curr->key);
			free(curr);
			curr = temp;
		}
	}else if (cmp < 0){
		curr->left = remove_tree_node(curr->left, key, data, 0, preceed);
	}else{
		curr->right = remove_tree_node(curr->right, key, data, 0, preceed);
	}

	if (balanced && curr) {
		// PARTIE 2 :
		int balance_factor = get_bfactor(curr);

		if (balance_factor > 1){
			if (get_bfactor(curr->left) >= 0){
				curr = rotate_right(curr);
			}else{
				curr->left = rotate_left(curr->left);
				curr = rotate_right(curr);
			}
		}else if (balance_factor < -1){
			if (get_bfactor(curr->right) <= 0){
				curr = rotate_left(curr);
			}else{
				curr->right = rotate_right(curr->right);
				curr = rotate_left(curr);
			}
		}
	}

	return curr;
}

/**
 * NB : Utiliser la fonction récursive remove_tree_node.
 */
void * tree_remove(struct tree_t * T, void * key) {
	assert(T != NULL && T->preceed != NULL);
	void *data = NULL;
	T->root = remove_tree_node(T->root, key, &data, T->balanced, T->preceed);

	return data;
}
