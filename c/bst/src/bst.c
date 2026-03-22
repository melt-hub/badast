
/* bst.c */

#include "bst/bst.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ERR "\033[31m"        /* ANSI Red.               */
#define WARN "\033[33m"       /* ANSI Yellow.            */
#define SUCC "\033[32m"       /* ANSI Green.             */
#define BLUE "\033[34m"       /* ANSI Blue.              */
#define CYAN "\033[36m"       /* ANSI Cyan.              */
#define MAGENTA "\033[35m"    /* ANSI Magenta.           */
#define COLOR_RESET "\033[0m" /* RESET Color to Default. */


#define TREE_V "│   "   /* UTF-8 Vertical pipe: connects valid paths.      */
#define TREE_L "└── "   /* UTF-8 L-shape: indicates the last child.        */
#define TREE_T "├── "   /* UTF-8 T-shape: indicates a child with siblings. */
#define TREE_S "    "   /* UTF-8 Empty space: indentation for 
                         * closed branches.                                */

/* node_t
 * 
 * A node is the structure that recursively defines a whole
 * binary tree. The field 'left' stores a pointer to another
 * node: the left child. The field 'right' store the pointer
 * to the right child. The 'key' field stores an integer: the
 * key. The 'value' field stores a pointer to a string. */
struct bst_node_t {
  bst_node left;
  bst_node right;
  bst_key_t key;
  bst_value_t value;
};

/* create_node 
 * 
 * This function:
 * (1) allocates memory for a new 'node_t'. 
 * (2) initializes the 'node.key' field at 'key'.
 * (3) initializes the 'node.value' field at 'value'.
 * (4) initializes the 'node.left field at 'left'.
 * (5) initializes the 'node.right field at 'right..*/
static bst_node create_node(bst_key_t key,
                            bst_value_t value,
                            bst_node left,
                            bst_node right) {
  bst_node node = (bst_node) malloc(sizeof(struct bst_node_t));
  if (!node) {
    free(node);
    fprintf(stderr, ERR "create_node: error:" COLOR_RESET
            " cannot allocate memory for new node.");
    exit(1);
  }
  node->key = key;
  node->value = value;
  node->left = left;
  node->right = right;
  return node;
}

/* bst_create
 *
 * An interface to that allows to create a BST with only the
 * root. */
bst_node bst_create(bst_key_t key, bst_value_t value) {
  return create_node(key, value, NULL, NULL);
}

/* bst_destroy
 * 
 * This function deallocates memory from a 'bst_node_t' struct
 * when only literal/constant strings are stored in it, thus only
 * the memory allocated to the struct's pointer's field needs
 * to be freed. 
 *
 * NOTE: this is because constant strings are stored in
 * the '.rodata' section of the program, which must be managed
 * by the operative system, not the user space processes.*/
void bst_destroy(bst_node root) {
  if (!root) {
    return;
  } else {
    bst_destroy(root->left);
    bst_destroy(root->right);
    free(root);
    return;
  } 
}

/* bst_destroy_deep
 *
 * This function deallocartes memory from a 'bst_node_t' struct
 * when only heap dynamic strings are stored in it, thus both the
 * struct's pointer's field and data stored at the address pointed
 * need to be deallocated. */
void bst_destroy_deep(bst_node root) {
  if (!root) {
    fprintf(stderr, ERR "error: bst: destroy_deep:" COLOR_RESET
            " can't deallocate memory from NULL pointer, "
            "the structure is empty.");
    return;
  } else {
    bst_destroy_deep(root->left);
    bst_destroy_deep(root->right);
    if (root->value) free(root->value);
    free(root);
    return;
  }
}

/* print_tree_recursive
 *
 * This internal helper function performs a pre-order traversal of the BST
 * to print nodes with hierarchical indentation.
 * 
 * To correctly draw vertical bars '│', the function maintains a history
 * of the ancestors' state using the 'ftr' (footprint) array.
 * 
 * @param root  : The current node to print.
 * @param level : Current depth level (0 = root).
 * @param ftr   : Integer array functioning as a boolean map.
 *                ftr[i] == 1 means the ancestor at level 'i' is the last
 *                child of its parent (no vertical bar needed).
 *                ftr[i] == 0 means the ancestor has following siblings
 *                (vertical bar needed). 
 *
 * NOTE: Google Gemini 3.0 Pro wrote this code. */
static void print_tree_recursive(bst_node root, int level, int *ftr) {
  if (!root) {
    return;
  }

  /* 1. Indentation Prefix 
   * Iterate through ancestors to draw the background grid. If an ancestor
   * was the last child, leave empty space. Otherwise, draw a vertical line
   * to connect to future nodes. */
  for (int i = 0; i < level - 1; i++) {
    printf(ftr[i] ? TREE_S : TREE_V);
  }

  /* 2. Node Connector 
   * Draw the immediate connection to the current node.
   * If root (level 0), no connector.
   * If last child (ftr[level-1] == 1), draw '└──'.
   * Otherwise, draw '├──'. */
  if (level > 0) {
    printf(ftr[level - 1] ? TREE_L : TREE_T);
  }

  /* 3. Print Data */
  /* If the value string exists, print it too, otherwise just the key. */
  if (root->value) {
      printf("(%d) -> %s\n", root->key, (char *)root->value);
  } else {
      printf("(%d)\n", root->key);
  }

  /* 4. Recursion Step 
   * Prepare the footprint for the next level. 
   * 
   * - Left Child: Is considered the "last" child for visualization 
   *   purposes ONLY if the right child is NULL.
   * - Right Child: Is always the last child to be visited. */
  
  /* Visit Left */
  if (root->left) {
    ftr[level] = (root->right == NULL) ? 1 : 0;
    print_tree_recursive(root->left, level + 1, ftr);
  }

  /* Visit Right */
  if (root->right) {
    ftr[level] = 1;
    print_tree_recursive(root->right, level + 1, ftr);
  }
}

/* bst_print
 * 
 * Wrapper function for the recursive pretty printer. It allocates
 * a static buffer 'ftr' (footprint) large enough to handle deep trees
 * without dynamic allocation overhead during printing. 
 *
 * NOTE: 2048 is an arbitrary safe limit for the tree depth recursion. */
void bst_print(bst_node root) {
  if (!root) {
    printf("(null)\n");
    return;
  }
  
  int ftr[2048]; /* Ancestor history buffer */
  ftr[0] = 1;    /* Root is conceptually the "last" of the starting context */
  
  print_tree_recursive(root, 0, ftr);
}

/* insert
 *
 * This function recursively descendes the 'bst_node_t' struct following
 * the BST property, until it finds the first node with its 'left' or 'right'
 * field (based on the input key the programmer is willing to insert) pointing
 * to NULL, and finally overwrites it with a pointer to a new node storing 
 * the input 'key' in its 'key' field. */
bst_node bst_insert(bst_node root, bst_key_t key, bst_value_t str) {
  if (!root) {
    return create_node(key, str, NULL, NULL);
  } else if (root->key > key) {
      root->left = bst_insert(root->left, key, str);
  } else {
      root->right = bst_insert(root->right, key, str);
  }
  return root;
}

/* insert path copy
 * 
 * This function */
bst_node bst_insert_pathcp(bst_node root, bst_key_t key, bst_value_t str) {
  if (root == NULL) {
    return create_node(key, str, NULL, NULL);
  } else if (root->key > key) {
    return create_node(root->key,
                       root->value,
                       bst_insert_pathcp(root->left, key, str),
                       root->right);
  } else {
    return create_node(root->key,
                       root->value,
                       root->left,
                       bst_insert_pathcp(root->right, key, str));
  }
}

/* delete
 *
 * This function recursively descends the 'bst_node_t' struct until
 * it finds the node that stores the value 'to_delete' in its key, 
 * then it operates in three different branches:
 *
 *   (a) if (root.left = NIL) AND (root.right = NIL)
 *   (b) elseif (root.left = NIL) OR (root.right = NIL)
 *   (c) else [root.left != NIL AND root.right != NIL]
 *
 * In branch (a), the node is a LEAF, and its memory is deallocated.
 * In branch (b), the node has AT LEAST one child, thus the struct is 
 * contracted [see contraction(bst_node]. In branch (c) the node
 * implicitly has both its children, so the integer stored in its 'key'
 * field is swapped with its successor's [see successor(bst_node)], and
 * the node that once stored its successor is deleted recursively. */
bst_node bst_delete(bst_node root, int to_delete) {
  if (!root) {
    return NULL;
  } else if (to_delete < root->key) {
    root->left = bst_delete(root->left, to_delete);
  } else if (to_delete > root->key) {
    root->right = bst_delete(root->right, to_delete);
  } else if (root->left == NULL) {
    bst_node temp = root->right;
    if (root->value) free(root->value);
    free(root); 
    return temp; 
  } else if (root->right == NULL) {
    bst_node temp = root->left;
    if (root->value) free(root->value);
    free(root);
    return temp;
  } else {
    bst_node succ = bst_min(root->right);
    if (root->value) free(root->value);
    root->key = succ->key;
    root->value = succ->value;
    succ->value = NULL;
    root->right = bst_delete(root->right, succ->key);
  }
  return root;
}

static char *safe_strdup(const char *s) {
    return s ? strdup(s) : NULL;
}

bst_node bst_delete_pathcp(bst_node node, bst_key_t key) {
  if (node == NULL) {
    return NULL;
  } else if (key < node->key) {
    return create_node(node->key,
                    safe_strdup(node->value),
                    bst_delete_pathcp(node->left, key),
                    node->right);                     
  } else if (key > node->key) {
    return create_node(node->key,
                    safe_strdup(node->value),
                    node->left,  
                    bst_delete_pathcp(node->right, key));
  } else {
    if (node->left == NULL) {
      return node->right;
    } else if (node->right == NULL) {
      return node->left;
    } else {
      bst_node succ = bst_min(node->right);
      return create_node(succ->key,
                      safe_strdup(succ->value),
                      node->left,
                      bst_delete_pathcp(node->right, succ->key));
    }
  }
}

bst_node bst_search(bst_node root, bst_key_t key) {
  if (!root) {
    return NULL;
  } else if (root->key > key) {
    return bst_search(root->left, key);
  } else if (root->key < key) {
    return bst_search(root->right, key);
  } else {
    return root;
  }
}

/* min
 * 
 * This function returns the pointer to the node storing
 * the minimum integer in its 'key' field. */
bst_node bst_min(bst_node root) {
  if (!root) {
    return NULL;
  } else if (root->left == NULL) {
    return root;
  } else {
    return bst_min(root->left);
  }
}

/* max
 * 
 * This function returns the pointer to the node storing
 * the maximum integer in its 'key' field. */
bst_node bst_max(bst_node root) {
  if (!root) {
    return NULL;
  } else if (root->right == NULL) {
    return root;
  } else {
    return bst_max(root->right);
  }
}

/* predecessor
 *
 * This function searches for the node storing the integer 'x'
 * in its 'key' field, then searches for the node storing x's 
 * successor in the 'bst_node_t' struct*/
bst_node bst_predecessor(bst_node root, bst_key_t key) {
  bst_node current = bst_search(root, key); 
  if (!current) {
    return NULL;
  } else if (current->left) {
    return bst_max(current->left);
  } else {
    bst_node predecessor = NULL;
    bst_node ancestor = root;
    while (ancestor != current) {
      if (key > ancestor->key) {
        predecessor = ancestor; 
        ancestor = ancestor->right;
      } else {
        ancestor = ancestor->left;
      }
    }
    return predecessor;
  }
}

/* successor
 *
 * This function searches for the node storing the integer 'x'
 * in its 'key' field, then searches for the node storing x's 
 * successor in the 'bst_node_t' struct*/
bst_node bst_successor(bst_node root, bst_key_t key) {
  bst_node current = bst_search(root, key); 
  if (!current) {
    return NULL;
  } else if (current->right != NULL) {
    return bst_min(current->right);
  } else {
    bst_node successor = NULL;
    bst_node ancestor = root;
    while (ancestor != current) {
      if (key < ancestor->key) {
        successor = ancestor; 
        ancestor = ancestor->left;
      } else {
        ancestor = ancestor->right;
      }
    }
    return successor;
  }
}

/* LCA - Lowest Common Ancestor
 * 
 * This function searches for the lowest node that has both the nodes storing
 * respectively 'k1' and 'k2' in their 'key' field as descendants.
 *
 * The lowest common ancestor can be described as the single node where the 
 * "path" to reach two different other nodes splits: in other words it is
 * the node where 'k1' and 'k2' stop being part of the same subtree. */
bst_node bst_lowest_common_ancestor(bst_node root, 
                                    bst_key_t k1, 
                                    bst_key_t k2) {
  if (root == NULL) {
    return NULL;
  } else if (root->key == k1 || root->key == k2) {
    return root;
  } else if (root->key > k1 && root->key < k2){
    return root;
  } else if (root->key > k2 && root->key < k1) {
    return root;
  } else {
    if (root->key > k1 && root->key > k2) {
      return bst_lowest_common_ancestor(root->left, k1, k2);
    } else if (root->key < k1 && root->key < k2) {
      return bst_lowest_common_ancestor(root->right, k1, k2);
    }
  }
}

/* Count Greater Than (Threshold)
 * 
 * This function recursively descends the BST searching and counting
 * all the occurrences of nodes whose 'key' field stores a number greater
 * than the 'trhold' parameter, then return the total count. */
int bst_count_greater_than(bst_node root, bst_key_t trhold) {
  if (root == NULL) {
    return 0;
  } else {
    int acc_right_subtree = bst_count_greater_than(root->right, trhold);
    int acc_left_subtree = 0;
    if (root->key > trhold) {
      acc_left_subtree = bst_count_greater_than(root->left, trhold);
    }
    int tot_accumulator = 0;
    if (root->key > trhold) {
       tot_accumulator = 1;
    }
    return tot_accumulator + acc_right_subtree + acc_left_subtree;
  }
}

/* Identical To
 * 
 * This function recursively simultaneously descends both the BSTs 
 * 'roo1' and 'root1' to check if they are identical both structurally
 * and in the value of the keys stored in them. If any structural or
 * mathematical equality check fails, the whole inference that the
 * two BSTs are identical collapses. */
int bst_identical_to(bst_node root1, bst_node root2) {
  if (root1 == NULL && root2 == NULL) { 
    return 1;
  } else if (root1 == NULL || root2 == NULL) {
    return 0;
  } else if (root1->key != root2->key) {
    return 0;
  } else {
    return bst_identical_to(root1->left, root2->left) 
           && bst_identical_to(root1->right, root2->right);
  }
}

/* Max Depth
 * 
 * This function recursively descends the left and right subtrees of
 * the root until reaching the leaves, keeping count of the number of
 * nodes visited and recursively confronts which walk was longer, thus
 * computing the longest walkable path in the BST. */
int bst_max_depth(bst_node root) {
  if (root == NULL) {
    return 0;
  } else {
    int left_subtree_depth = 1 + bst_max_depth(root->left);
    int right_subtree_depth = 1 + bst_max_depth(root->right);
    if (left_subtree_depth > right_subtree_depth) {
      return left_subtree_depth;
    } else {
      return right_subtree_depth;
    }
  }
}

/* Mirror
 * 
 * This function botto-to-top, recursively mirrors the left and right 
 * child of each node in the BST 'root', thus mirroring the whole structure
 * and inverting the BST property. */
void bst_mirror(bst_node root) {
  if (root == NULL) {
    return;
  } else if (root->left == NULL && root->right == NULL) {
    return;
  } else {
    bst_mirror(root->left);
    bst_mirror(root->right);
    bst_node tmp = root->left;
    root->left = root->right;
    root->right = tmp;
    return;
  }
}

/* Path Sum
 *
 * This function recursively descends the left and right child of each node
 * while decrementing the parameter 'target' by 'root.key' until it exactly 
 * zeroes 'target', note that it could fail doing so. */
int bst_path_sum(bst_node root, int target) {
  if (root == NULL) {
    return 0;
  } else if (root->left == NULL && root->right == NULL) {
    return target == root->key;
  } else {
    return bst_path_sum(root->left, target - root->key) 
           || bst_path_sum(root->right, target - root->key);
  }
}

/* Sor(ted)Vec(tor) Convert
 * 
 * This function applies the Divide and Conquer paradigm to recursively
 * split in half the vector, then inserting the element in the middle of
 * each splitted section in a BST in pre-order, thus building a balanced
 * BST from a linear vector. */
bst_node bst_sorvec_convert(int *sorvec, int l, int r) {
  if (r < l) {
    return NULL;
  } else {
    int m = (l + r) / 2;
    bst_node t = create_node(sorvec[m], NULL, NULL, NULL);
    t->key = sorvec[m];
    t->left = bst_sorvec_convert(sorvec, l, m - 1);
    t->right = bst_sorvec_convert(sorvec, m + 1, r);
    return t;
  }
}

bst_node bst_kth_smallest(bst_node root, bst_key_t trhold) {
}

/* Check (if it's a BST)
 *
 * This function recursively visits the left and right child of each node
 * while checking if the integer stored in its 'key' field respects the 
 * BST property. In order to do that it is not enough to just check the 
 * mathematical relation between <root.left.key><root.key><root.right.key>,
 * but it is necessary to keep track of the infimum and supremum of the set
 * of possible integers that can be stored in each node descending in the
 * BST. */
int bst_check(bst_node root, bst_key_t inf, bst_key_t sup) {
  if (root == NULL) {
    return 1;
  } else {
    if (root->key <= inf || root->key > sup) { 
      return 0;
    } else {
      return bst_check(root->left, inf, root->key)
             &&
             bst_check(root->right, root->key, sup);
    }
  }
}

/* end of bst.c */
