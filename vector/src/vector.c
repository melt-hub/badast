
/* ===========================| --------------- |=============================
 *
 * (Dynamic) Vector
 *
 * This file provides implementation for a set of basic operation computable
 * on a dynamic vector.
 *
 * ===========================| --------------- |=============================*/

#include "vector/vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INIT_CAP 4      /* Initial Capacity. */
#define GROW_TRESH 1000 /* Grow Treshold.    */

#define ERR "\033[31m"        /* ANSI Red.     */
#define WARN "\033[33m"       /* ANSI Yellow.  */
#define SUCC "\033[32m"       /* ANSI Green.   */
#define BLUE "\033[34m"       /* ANSI Blue.    */
#define CYAN "\033[36m"       /* ANSI Cyan.    */
#define MAGENTA "\033[35m"    /* ANSI Magenta. */
#define COLOR_RESET "\033[0m" /* RESET Color to Default. */

/* vector_t
 *
 * The char **vector field implements the actual dictionary: a pointer to a 
 * string is the key, the string itself is the value. So this dictionary
 * stores couples of <char **, char *>.
 * 
 * The vector_capacity_t capacity field represents space available for
 * allocation.
 *
 * The vector_size_t size field represent the actual number of elements
 * stored. */
struct vector_t {
  char **vector;
  vector_capacity_t capacity;
  vector_size_t size;
};

/* create
 * 
 * This function: 
 * (1) allocates memory for a new dynamic vector. 
 * (2) initializes the vector field with the capacity to store one string. 
 * (3) initializes size to the number of currently stored elements (zero). */
vector vector_create() {
  vector newvec = (vector) malloc(sizeof(struct vector_t));
  if (!newvec) {
    fprintf(stderr, ERR "vector: error: create:" COLOR_RESET 
            " cannot allocate new vector.\n");
    exit(1);
  }
  newvec->vector = (char **) malloc(INIT_CAP * sizeof(char *));
  if (!newvec->vector) {
    free(newvec);
    fprintf(stderr, ERR "vector: error: create:" COLOR_RESET 
            " cannot allocate field vector.\n");
    exit(1);
  }
  newvec->capacity = INIT_CAP;
  newvec->size = 0;
  return newvec;
}

/* grow
 * 
 * This function estimates how much extra memory the 'vector' field needs and 
 * reallocates it. */
static int grow(vector v) {
  if (!v) {
    fprintf(stderr, ERR "error: vector: grow:" COLOR_RESET 
            " can't grow a null vector.\n");
    exit(1);
  }
  vector_capacity_t nc;
  if (v->capacity <= GROW_TRESH) {
    nc = v->capacity * 2;
  } else {
    nc = (vector_capacity_t) v->capacity * 1.5;
  }
  char ** nv = (char **) realloc(v->vector, nc * sizeof(char *));
  if (!nv) {
    fprintf(stderr, ERR "error: vector: grow:" COLOR_RESET
            " can't grow vector because process"
            " out of memory.\n");
    return 0;
  }
  v->vector = nv;
  v->capacity = nc;
  return 1;
}

/* destroy
 * 
 * This functions deallocates memory from a vector_t struct when only 
 * literal/constant strings are stored in it, thus only pointer
 * deallocation is needed.
 *
 * NOTE: this is because constant strings are stored in
 * the '.rodata' section of the program, which must be managed
 * by the operative system, not the processes. */
void vector_destroy(vector v) {
  if (!v) {
    return;
  }
  if (!(!v->vector)) {
    free(v->vector);
  }
  free(v);
}

/* destroy deep
 * 
 * This functions deallocates memory from a vector_t struct when only 
 * heap dynamic strings are stored in it, thus both pointer and data
 * stored at the address pointed need to be deallocated. */
void vector_destroy_deep(vector v) {
  if (!v || !v->vector) {
    return;
  }
  for (vector_size_t i = 0; i < v->size; i++) {
    free(v->vector[i]);
  }
  free(v->vector);
  free(v);
}

/* print index (iterative)
 *
 * This function iteratively prints the capacity, size and elements 
 * of a vector_t struct, and represents each address by an integer
 * subscript. */
void vector_print(vector v) {
  printf(BLUE "{size:" COLOR_RESET
         CYAN "%ld" COLOR_RESET
         BLUE ",capacity:" COLOR_RESET
         CYAN "%ld" COLOR_RESET
         BLUE ",space:" COLOR_RESET
         CYAN "%ld(bytes)" COLOR_RESET
         BLUE "}\n" COLOR_RESET
         ,v->size 
         ,v->capacity
         ,vector_getbytes(v));
  if (vector_empty(v)) {
    printf(BLUE "(" COLOR_RESET
           CYAN "null" COLOR_RESET
           BLUE ")\n" COLOR_RESET);
  }
  for (vector_size_t i = 0; i < v->size; i++) {
    printf(BLUE "[" COLOR_RESET
           CYAN "%ld" COLOR_RESET 
           BLUE "] -> [" COLOR_RESET
           CYAN "%s" COLOR_RESET
           BLUE "]\n" COLOR_RESET,
           i, 
           v->vector[i]);
  }
  return;
}

/* print index (tail recursive)
 *
 * This function recursively prints the capacity, size and elements 
 * of a vector_t struct, and represents each subscript by a logical
 * integer index. */
static void vector_print_tail_ind(vector v, vector_size_t i) {
  if (i >= v->size) {
    return;
  } else {
    printf(BLUE "[" COLOR_RESET
           CYAN "%ld" COLOR_RESET 
           BLUE "] -> [" COLOR_RESET
           CYAN "%s" COLOR_RESET
           BLUE "]\n" COLOR_RESET,
           i, 
           v->vector[i]);
    return vector_print_tail_ind(v, i + 1);
  } 
}

/* print address (tail recursive)
 *
 * This function recursively prints the capacity, size and elements 
 * of a vector_t struct, and represents each subscript by its hexadecimal
 * address. */
static void vector_print_tail_adr(vector v, char **r) {
  if (r >= v->vector + v->size) {
    return;
  } else {
    printf(BLUE "[" COLOR_RESET
           CYAN "%p" COLOR_RESET 
           BLUE"] -> [" COLOR_RESET
           CYAN "%s" COLOR_RESET
           BLUE "]\n" COLOR_RESET,
           r, 
           *r);

    return vector_print_tail_adr(v, r + 1);
  }
}

/* print_tr (starter)
 *
 * Starter function for both print_tail implementations. The function takes
 * a logical binary value (eight bit that can be either 0 or any integer
 * smaller that {2^8 - 1}). Zero calls the print_tail_int implementation.
 * One calls the print_tail_add implementation. */
void vector_print_tr(vector v, uint8_t s) {
  printf(BLUE "{size:" COLOR_RESET
         CYAN "%ld" COLOR_RESET
         BLUE ",capacity:" COLOR_RESET
         CYAN "%ld" COLOR_RESET
         BLUE ",space:" COLOR_RESET
         CYAN "%ld(bytes)" COLOR_RESET
         BLUE "}\n" COLOR_RESET
         ,v->size 
         ,v->capacity
         ,vector_getbytes(v));
  if (!v) {
    printf(BLUE "(" COLOR_RESET
           CYAN "null" COLOR_RESET
           BLUE ")\n" COLOR_RESET);
  }
  if (s == 0) {
    return vector_print_tail_ind(v, 0);
  } else {
    return vector_print_tail_adr(v, v->vector);
  }
}

/* prepend (helper)
 * 
 * Helper function that checks and manages exception and errors in 
 * prepend's parameters, return values:
 *   0: Error
 *   1: Handled
 *   2: Valid & Ready
 */
static int prepend_check_param(vector v, char *str) {
  if (!v) {
    fprintf(stderr, ERR "error: vector: prepend:" COLOR_RESET
            " can't append to a null vector.\n");
    return 0;
  } 
  if (!str) {
    printf(WARN "warning: vector: prepend:" COLOR_RESET
          " null string, nothing added.\n");
    return 0;
  }
  if (v->size == 0) {
    printf(WARN "warning: vector: prepend:" COLOR_RESET
           " size is zero, appended (%s).\n"
           , str);
    return vector_append(v, str) ? 1 : 0;
  }
  if (v->size == v->capacity) {
    if (!grow(v)) {
      return 0;
    }
  }
  return 2;
}

/* prepend (iterative)
 * 
 * This function iteratively head inserts the element 'str'. */
int vector_prepend(vector v, char *str) {
  int s = prepend_check_param(v, str);
  if (s == 0) return 0;
  if (s == 1) return 1;

  for (vector_size_t i = v->size; i > 0; i--) {
    v->vector[i] = v->vector[i - 1];
  }
  v->vector[0] = str;
  v->size++;
  return 1;
}

/* prepend (iterative, optimized)
 * 
 * This function head inserts the element 'str' in time complexity O(n). */
int vector_prepend_opt(vector v, char *str) {
  int s = prepend_check_param(v, str);
  if (s == 0) return 0;
  if (s == 1) return 1;

  memmove(&v->vector[1], 
          &v->vector[0], 
          v->size * sizeof(char*));
  v->vector[0] = str;
  v->size++;
  return 1;
}

/* prepend (tail recursive)
 *
 * This function recursively head inserts the element 'str'. */
static int vector_prepend_tail(vector v, char *str, vector_size_t p) {
  if (p == 0) {
    v->vector[p] = str;
    v->size++;
    return 1;
  } else {
    v->vector[p] = v->vector[p - 1];
    return vector_prepend_tail(v, str, p - 1);
  }
}

/* prepend_tr (starter)
 * 
 * Starter function for prepend_tail. */
int vector_prepend_tr(vector v, char *str) {
  int s = prepend_check_param(v, str);
  if (s == 0) 
    return 0;
  else if (s == 1) 
    return 1;
  else 
    return vector_prepend_tail(v, str, v->size);
}

/* append (iterative)
 * 
 * This function iteratevely tail inserts the element 'str'. */
int vector_append(vector v, char *str) {
  if (!v) {
    fprintf(stderr, ERR "error: vector: append:" COLOR_RESET
            " can't append to a null vector.\n");
    return 0;
  } 
  if (!str) {
    printf(WARN "warning: vector: append:" COLOR_RESET 
           "null string, nothing added.\n");
    return 0;
  }
  if (v->size == v->capacity) {
    grow(v);
  }
  v->vector[v->size] = str;
  v->size++;
  return 1;
}

/* insert (helper)
 * 
 * Helper function that checks and manages exception and errors in 
 * insert's parameters, return values:
 *   0: Error
 *   1: Handled
 *   2: Valid & Ready
 */
static int insert_check_param(vector v, vector_size_t p, char *str) {
  if (!v) {
    fprintf(stderr, ERR "error: vector: insert:" COLOR_RESET
           " can't insert into a null vector.\n");
    return 0;
  }
  if (!str) {
    printf(WARN "warning: vector: insert:" COLOR_RESET 
           " null string, nothing added.\n");
    return 0;
  }
  if (p == 0) {
    printf(WARN "warning: vector: insert:" COLOR_RESET
           " position provided (%ld) was zero,"
           " prepended (%s).\n", p, str);
    return vector_prepend(v, str) ? 1 : 0;
  }
  if (p >= v->size) {
    printf(WARN "warning: vector: insert:" COLOR_RESET 
           " position provided (%ld) was greater"
           " than vector size (%ld), appended (%s).\n", p, v->size, str);
    return vector_append(v, str) ? 1 : 0;
  }
  if (v->size == v->capacity) {
    if (grow(v) == 0) {
      return 0;
    }
  }
  return 2;
}

/* insert (iterative)
 * 
 * This function iteratively inserts an element 'str' at position 'p'. */
int vector_insert(vector v, vector_size_t p, char *str) {
  int s = insert_check_param(v, p, str);
  if (s == 0) return 0;
  if (s == 1) return 1;

  vector_size_t i = v->size;
  while (i > p) {
    v->vector[i] = v->vector[i - 1];
    i--;
  }
  v->vector[p] = str;
  v->size++;
  return 1;
} 

/* insert (iterative, optimized)
 * 
 * This function iteratively inserts an element 'str' at position 'p'
 * in time complexity O(n). */
int vector_insert_opt(vector v, vector_size_t p, char *str) {
  int s = insert_check_param(v, p, str);
  if (s == 0) return 0;
  if (s == 1) return 1;

  memmove(&v->vector[p + 1],
          &v->vector[p], 
          (v->size - p) * sizeof(char*));
  v->vector[p] = str;
  v->size++;
  return 1;
}

/* insert (tail recursive)
 * 
 * This function tail recursively inserts the element 'str' at 
 * position 'p'. */
static int vector_insert_tail(vector v, 
                              vector_size_t p, 
                              char *str, 
                              vector_size_t i) {
  if (i == p) {
    v->vector[i] = str;
    v->size++;
    return 1;
  } else {
    v->vector[i] = v->vector[i - 1];
    return vector_insert_tail(v, p, str, i - 1);
  }
}

/* insert (starter)
 * 
 * Starter function for insert_tail. */
int vector_insert_tr(vector v, vector_size_t p, char *str) {
  int s = insert_check_param(v, p, str);
  if (s == 0) 
    return 0;
  else if (s == 1) 
    return 1;
  else 
    return vector_insert_tail(v, p, str, v->size);
}

/* delete (helper)
 * 
 * Helper function that checks and manages exception and errors in 
 * delete's parameters, return values:
 *   0: Error
 *   1: Handled
 */
static int delete_check_param(vector v, vector_size_t p) {
  if (!v) {
    fprintf(stderr, ERR "error: vector: delete:" COLOR_RESET
            " can't delete from a null vector.\n");
    return 0;
  }
  if (p > v->size) {
    fprintf(stderr, ERR "error: vector: delete:" COLOR_RESET
            " can't delete element at %ld, index out of bounds.\n",
            p);
    return 0;
  }
  if (!v->vector[p]) {
    fprintf(stderr, ERR "error: vector: delete:" COLOR_RESET
            " can't delete element at %ld," 
            " element is null.\n", 
            p);
    return 0;
  }
  return 1;
}

/* delete (iterative)
 *
 * This function deletes the element stored at position 'p'. */
int vector_delete(vector v, vector_size_t p) {
  if (delete_check_param(v, p) == 0) return 0;

  free(v->vector[p]); 
  for (vector_size_t i = p; i < v->size - 1; i++) {
    v->vector[i] = v->vector[i + 1];
  }
  v->size--;
  return 1;
}

/* delete (iterative, optimized)
 * 
 * This function deletes the element stored at position 'p' 
 * in time complexity O(n). */
int vector_delete_opt(vector v, vector_size_t p) {
  if (delete_check_param(v, p) == 0) return 0;

  free(v->vector[p]);
  memmove(&v->vector[p],
          &v->vector[p + 1],
          (v->size - p) * sizeof(char *));
  v->size--;
  return 1;
}

/* delete (tail recursive) 
 * 
 * This function deletes the element stored at position 'p' 
 * in time complexity O(n). */
static int vector_delete_tail(vector v, vector_size_t p, vector_size_t i) {
  if (delete_check_param(v, p) == 0) return 0;

  if (i >= v->size) {
    v->size--;
    return 1;
  } else {
    v->vector[i] = v->vector[i + 1];
    return vector_delete_tail(v, p, i + 1);
  }
}

/* delete (starter)
 * 
 * Starter function for delete_tail. */
int vector_delete_tr(vector v, vector_size_t p) {
  if (delete_check_param(v, p) == 0) return 0;

  return vector_delete_tail(v, p, p);
}

/* remove (iterative)
 *
 * This function extracts the element stored at position 'p' and returns it. */
char * vector_remove(vector v, vector_size_t p) {
  if (!v) {
    fprintf(stderr, ERR "error: vector: remove:" COLOR_RESET
            " can't remove from a null vector.\n");
    return 0;
  }
  if (p > v->size) {
    fprintf(stderr, ERR "error: vector: remove:" COLOR_RESET
            " can't remove element at %ld, index out of bounds.\n",
            p);
    return 0;
  }
  if (!v->vector[p]) {
    fprintf(stderr, ERR "error: vector: remove:" COLOR_RESET
            " can't remove element at %ld, element is null.\n", 
            p);
    return 0;
  }
  char * r = v->vector[p];
  for (vector_size_t i = p; i < v->size - 1; i++) {
    v->vector[i] = v->vector[i + 1];
  }
  v->size--;
  return r;
}

/* delete_first (iterative)
 *
 * This function deletes the element stored at position 0. */
int vector_delete_first(vector v) {
  if (!v) {
    fprintf(stderr, ERR "error: vector: delete_first:" COLOR_RESET
            " can't delete from a null vector.\n");
    return 0;
  }
  if (vector_empty(v)) {
    fprintf(stderr, ERR "error: vector: delete_first:" COLOR_RESET
            " can't delete from an empty vector.\n");
    return 0;
  }
  free(v->vector[0]);
  for (vector_size_t i = 0; i < v->size - 1; i++) {
    v->vector[i] = v->vector[i + 1];
  }
  v->size--;
  return 1;
}

/* remove_first (iterative)
 *
 * This function extracts the element stored at position 0 and returns it. */
char * vector_remove_first(vector v) {
  if (!v) {
    fprintf(stderr, ERR "error: vector: remove_first:" COLOR_RESET
            " can't remove from a null vector.\n");
    return 0;
  }
  if (vector_empty(v)) {
    fprintf(stderr, ERR "error: vector: remove_first:" COLOR_RESET
            " can't remove from an empty vector.\n");
    return 0;
  }
  char * r = v->vector[0];
  for (vector_size_t i = 0; i < v->size - 1; i++) {
    v->vector[i] = v->vector[i + 1];
  }
  v->size--;
  return r;
}

/* delete_last (iterative)
 *
 * This function deletes the element stored at position v->size - 1 */
int vector_delete_last(vector v) {
  if (!v) {
    fprintf(stderr, ERR "error: vector: delete_last:" COLOR_RESET
            " can't delete from a null vector.\n");
    return 0;
  }
  if (vector_empty(v)) {
    fprintf(stderr, ERR "error: vector: delete_last:" COLOR_RESET
            " can't delete from an empty vector.\n");
    return 0;
  }
  free(v->vector[v->size - 1]);
  v->size--;
  return 1;
}

/* remove_last (iterative)
 *
 * This function extracts the element stored at position 0 and returns it. */
char * vector_remove_last(vector v) {
  if (!v) {
    fprintf(stderr, ERR "error: vector: remove_last:" COLOR_RESET
            " can't remove from a null vector.\n");
    return 0;
  }
  if (vector_empty(v)) {
    fprintf(stderr, ERR "error: vector: remove_last:" COLOR_RESET
            " can't remove from an empty vector.\n");
    return 0;
  }
  char * r = v->vector[v->size - 1];
  v->size--;
  return r;
}

/* delete_all (iterative)
 * 
 * This function deletes all instances of 'str' from the vector*/
int vector_delete_all(vector v, char *str) {
  if (!v) {
    fprintf(stderr, ERR "error: vector: delete_all:" COLOR_RESET
            " can't delete from a null vector.\n");
    return 0;
  }
  if (vector_empty(v)) {
    fprintf(stderr, ERR "error: vector: delete_all:" COLOR_RESET
            " can't delete from an empty vector.\n");
    return 0;
  }
  vector_size_t s = v->size;
  vector_size_t w = 0;
  for (vector_size_t r = 0; r < v->size; r++) {
    if (strcmp(v->vector[r], str) != 0) {
      v->vector[w] = v->vector[r];
      w++;
    }
  }
  v->size = w;
  return (int) (s - v->size);
}

/* search (iterative)
 * 
 * This function researches the element 'str' and returns its position 
 * (if found), otherwhise it returns -1 */
int vector_search(vector v, char *str) {
  if (!v) {
    fprintf(stderr, ERR "error: vector: search:" COLOR_RESET 
            " can't search in a null vector.\n");
    return -1;
  }
  if (vector_empty(v)) {
    fprintf(stderr, ERR "error: vector: search:" COLOR_RESET
            " can't search in an empty vector.\n");
    return 0;
  }
  vector_size_t i = 0;
  while ((i < v->size) && (strcmp(v->vector[i], str) != 0)) {
    i++;
  }
  if (i < v->size) {
    return (int) i;
  } else {
    return -1;
  }
}

/* getsize
 *
 * This function returns the value stored in the field `size` */
vector_size_t vector_getsize(vector v) {
  if (!v) {
    printf(WARN "warning: vector: getsize:" COLOR_RESET 
           " size zero, vector is null.\n");
    return 0;
  } else {
    return v->size;
  }
}

/* getcapacity
 *
 * This function returns the value stored in the field `capacity` */
vector_capacity_t vector_getcapacity(vector v) {
  if (!v) {
    printf(WARN "warning: vector: getcapacity:" COLOR_RESET 
           " capacity zero, vector is null.\n");
    return 0;
  } else {
    return v->capacity;
  }
}

/* getat
 *
 * This function returns the element stored in the vector at position 'p' */
char * vector_getat(vector v, vector_size_t p) {
  if (!v) {
    fprintf(stderr, ERR "error: vector: getat:" COLOR_RESET
            " can't locate element, vector is null.\n");
    return NULL;
  }
  if (p >= v->size) {
    fprintf(stderr, ERR "error: vector: getat:" COLOR_RESET
            " can't locate element, index out of bounds.\n");
    return NULL;
  }
  return v->vector[p];
}

/* getbytes
 *
 * This function returns the size of a vector in bytes. */
size_t vector_getbytes(vector v) {
  if (!v) {
    printf(WARN "warning: vector: getbytes:" COLOR_RESET 
           " zero bytes allocated, vector is null.\n");
    return 0;
  }
  size_t t = sizeof(struct vector_t);
  if (!(!v->vector)) {
    t += v->capacity * sizeof(char *);
    for (vector_size_t i = 0; i < v->size; i++) {
      t += (strlen(v->vector[i]) + 1);
    }
  }
  return t;
}

/* empty
 *
 * This function checks wether the vector size's zero or not */
int vector_empty(vector v) {
  if (!v || v->size == 0) {
    return 1;
  } else {
    return 0;
  }
}

// end of vector.c
