
/* ===========================| --------------- |============================
 *
 * Binary Search Tree
 *
 * A BST (Binary Search Tree) is an ADT, organized in a binary tree, where 
 * keys are stored in such a way as to satisfy the "binary-search-tree
 * property", let be 'x' be a node:
 *
 *    - If 'y' is a node in the LEFT subtree of 'x', then 'y.key <= x.key'.
 *    - If 'z' is a node in the RIGHT subtree of 'x', then 'z.key >= xkey'.
 *
 * This property grants a computational complexity of O(log n) for the main
 * operations, due to the fact that "half of the nodes" can be excluded when
 * exploring the tree, basing decision on the value of the key stored in 
 * the visited node. Computational complexity can worsen up to O(n) when 
 * visiting each node is required or the tree is unbalanced.
 *
 * A binary search tree can be represented as:
 *
 *                       root ->  [l, k, N, r]
 *                                 /        \
 *                         [l, k, p, r]  [l, p, k, r]
 *                         /         \   /         \
 *                     ...            ...             ...
 *               [NIL, k, p, NIL]              [NIL, p, k, NIL]
 *
 *  This header file provides the declaration of both the structured data and
 *  the functions implementing basic operations and algorithms computable on
 *  a BST, such as:
 *    
 *    - insert(T, x): inserts a node storing the key 'x' in the tree T.
 *    - delete(T, x): deletes the node storing the value 'x' in its key.
 *    - search(T, x): searches for the node storing the value 'x' in its key.
 *  
 *  But also:
 *
 *    - min(T): searches for the node storing the smallest value in its key.
 *    - max(T): searches for the node storing the biggest value in its key.
 *    - predecessor(T, x): searches for the node storing x's supremum value
 *                         in its key.
 *    - successor(T, x): searches for the node storing x's infimum value in
 *                       its key.
 *                         
 * ===========================| --------------- |============================ */

#ifndef BST_H
#define BST_H

#include <stddef.h>

struct bst_node_t;
typedef struct bst_node_t *bst_node;

typedef int bst_key_t;
typedef char *bst_value_t;

bst_node bst_create(bst_key_t, bst_value_t);
void bst_destroy();
void bst_destroy_deep();
void bst_print();

bst_node bst_insert(bst_node, bst_key_t, bst_value_t);
bst_node bst_insert_pathcp(bst_node, bst_key_t, char*);

bst_node bst_delete(bst_node, bst_key_t);
bst_node bst_delete_pathcp(bst_node, bst_key_t);

bst_node bst_search(bst_node, bst_key_t);

bst_node bst_min(bst_node);

bst_node bst_max(bst_node);

bst_node bst_predecessor(bst_node, bst_key_t);

bst_node bst_successor(bst_node, bst_key_t);

bst_node bst_lowest_common_ancestor(bst_node, bst_key_t, bst_key_t);

int bst_count_greater_than(bst_node, bst_key_t);

int bst_identical_to(bst_node, bst_node);

int bst_max_depth(bst_node);

void bst_mirror(bst_node);

int bst_path_sum(bst_node, int);

bst_node bst_sorvec_convert(int *, int, int);

bst_node bst_kth_smallest(bst_node, bst_key_t);

int bst_check(bst_node, bst_key_t, bst_key_t);

#endif

/* end of bst.h */
