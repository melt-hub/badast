/* ============================| --------------- |============================
 *
 * (Dynamic) Vector 
 *
 * A dynamic vector is implemented using an actual non-dynamic array, but 
 * provides the user with a useful set of operations. Although it may seem
 * that the computational complexity of almost any operation on a dynamic
 * vector is slightly worse than that of a linked list's, the hidden advantage
 * of an ADT mapping contiguous memory cells are many:
 *
 *   - while still O(n), usually operations computed on a dynamic vector 
 *     are 10x to 50x less costly due to more frequent cache hits. 
 *   - O(1) random accesses. 
 *
 * A dynamic vector can be represented as:
 *
 *           [elm][elm][elm][elm][elm][elm][elm][elm][elm][elm]    
 *             ^    ^    ^    ^    ^    ^    ^    ^    ^    ^
 *          [[ptr][ptr][ptr][ptr][ptr][ptr][ptr][ptr][ptr][ptr]]
 *             0                     ...                    n
 *          [1][3][4][5][5]
 *
 * This header file provides the declaration of both the structured data and 
 * the functions implementing basic operations and algorithms computable
 * on a dynamic vector, such as:
 * 
 *   - insert(x, i): inserts the key 'x' in position 'i'.
 *   - delete(x)   : removes the key 'x' from the list.
 *   - search(x)   : searches for the key 'x' in the list and
 *                   returns its position.
 *
 * And also:
 *
 *   - prepend(x)    : head inserts the key 'x'.
 *   - append(x)     : tail inserts the key 'x'.
 *   - deleteFirst(L): removes the first key from the list.
 *   - deleteLast(L) : removes the last key from the list.
 *   - deleteAll(x)  : removes every key 'x' from the list.
 *   - empty()       : checks if the list is empty.  
 *
 * ============================| --------------- |============================*/

#ifndef VECTOR_H 
#define VECTOR_H

#include <stddef.h>
#include <stdint.h>

struct vector_t;
typedef struct vector_t * vector;

typedef size_t vector_capacity_t;
typedef size_t vector_size_t;

vector vector_create();
void vector_destroy(vector);
void vector_destroy_deep(vector);
void vector_print(vector);
void vector_print_tr(vector, uint8_t);

int vector_prepend(vector, char *);
int vector_prepend_opt(vector, char *);
int vector_prepend_tr(vector, char *);

int vector_append(vector, char *);

int vector_insert(vector, vector_size_t, char *);
int vector_insert_opt(vector, vector_size_t, char *);
int vector_insert_tr(vector, vector_size_t, char *);

int vector_delete(vector, vector_size_t);
int vector_delete_opt(vector, vector_size_t);
int vector_delete_tr(vector, vector_size_t);

char * vector_remove(vector, vector_size_t);
char * vector_remove_tr(vector, vector_size_t);

int vector_delete_first(vector);
int vector_delete_first_tr(vector);

char * vector_remove_first(vector);
char * vector_remove_first_tr(vector);

int vector_delete_last(vector);
int vector_delete_last_tr(vector);

char * vector_remove_last(vector);
char * vector_remove_last_tr(vector);

int vector_delete_all(vector, char *);
int vector_delete_all_tr(vector, char *);

int vector_search(vector, char *);
int vector_search_tr(vector, char *);

int vector_empty(vector);
vector_size_t vector_getsize(vector);
char * vector_getat(vector, vector_size_t);
vector_capacity_t vector_getcapacity(vector);
size_t vector_getbytes(vector);

#endif
// end of vector.h
