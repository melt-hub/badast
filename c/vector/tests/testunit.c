
/* test file for vector.c */

#include "vector/testvector.h"
#include "vector/vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MIN_ARGS 2
/* ANSI Red. */
#define ERR "\033[31m"
/* ANSI Yellow. */
#define WARN "\033[33m"
/* ANSI Green. */
#define SUCC "\033[32m"
/* ANSI Blue. */
#define BLUE "\033[34m"
/* ANSI Cyan. */
#define CYAN "\033[36m"
/* Ansi Magenta. */
#define MAGENTA "\033[35m"

#define COLOR_RESET "\033[0m"

/* print_testheader
 * 
 * Helper function that prints a title for a test. */
static void print_testheader(const char *name) {
  printf(">>> " 
         MAGENTA "[TESTING]" COLOR_RESET 
         " \"%s\"\n"
         , name);
}
/* print_testpass
 *
 * Helper function that informs the user a test passed. */
static void print_testpass() {
  printf(">>> " 
         SUCC "[SUCCESS]" COLOR_RESET
         " Test passed!\n\n");
}

void test_create_destroy() {
  print_testheader("Creation & Destruction");
  vector v = vector_create();
  assert(v != NULL);
  assert(vector_empty(v));
  vector_print(v);
  vector_destroy(v);
  print_testpass();
}

void test_print_tr() {
  print_testheader("Print Index (Tail Recursive)");
  vector v = vector_create();
  vector_append(v, strdup("Gesù"));
  vector_append(v, strdup("Merda"));
  vector_append(v, strdup("Infame"));
  vector_append(v, strdup("Pestata"));
  vector_print_tr(v, 0);
  print_testheader("Print Address (Tail Recursive)");
  vector_print_tr(v, 1);
  vector_destroy(v);
  print_testpass();
}

void test_prepend(){
  print_testheader("Prepend (Iterative)");
  vector v = vector_create();
  vector_prepend(v, strdup("Cane"));
  vector_prepend(v, strdup("Dio"));
  assert(vector_getsize(v) == 2);
  assert(strcmp(vector_getat(v, 0), "Dio") == 0);
  assert(strcmp(vector_getat(v, 1), "Cane") == 0);
  vector_print(v);
  vector_destroy_deep(v);
  print_testpass();
}

void test_prepend_opt(){
  print_testheader("Prepend (Optimized)");
  vector v = vector_create();
  vector_prepend_opt(v, strdup("Cane"));
  vector_print_tr(v, 0);
  vector_prepend_opt(v, strdup("Dio"));
  vector_print_tr(v, 0);
  assert(vector_getsize(v) == 2);
  assert(strcmp(vector_getat(v, 0), "Dio") == 0);
  assert(strcmp(vector_getat(v, 1), "Cane") == 0);
  vector_destroy_deep(v);
  print_testpass();
}

void test_prepend_tr(){
  print_testheader("Prepend (Tail Recursive)");
  vector v = vector_create();
  vector_prepend_tr(v, strdup("Cane"));
  vector_print_tr(v, 0);
  vector_prepend_tr(v, strdup("Dio"));
  vector_print_tr(v, 0);
  assert(vector_getsize(v) == 2);
  assert(strcmp(vector_getat(v, 0), "Dio") == 0);
  assert(strcmp(vector_getat(v, 1), "Cane") == 0);
  vector_destroy_deep(v);
  print_testpass();
}

void test_append(){
  print_testheader("Append");
  vector v = vector_create();
  vector_append(v, strdup("Porco"));
  vector_append(v, strdup("Dio"));
  assert(vector_getsize(v) == 2);
  assert(strcmp(vector_getat(v, 0), "Porco") == 0);
  assert(strcmp(vector_getat(v, 1), "Dio") == 0);                              
  vector_print(v);
  vector_destroy_deep(v);
  print_testpass();
}

void test_insert(){  
  print_testheader("Insert");
  vector v = vector_create();
  vector_append(v, strdup("Dio"));
  vector_append(v, strdup("Cane"));
  vector_append(v, strdup("Mostruoso"));
  vector_print(v);
  vector_insert(v, 1, strdup("Bastardo"));
  assert(vector_getsize(v) == 4);
  assert(strcmp(vector_getat(v, 1), "Bastardo") == 0);
  vector_print(v);
  vector_destroy_deep(v);
  print_testpass();
}

void test_insert_opt(){  
  print_testheader("Insert (Optimized)");
  vector v = vector_create();
  vector_append(v, strdup("Dio"));
  vector_append(v, strdup("Cane"));
  vector_append(v, strdup("Mostruoso"));
  vector_print(v);
  vector_insert_opt(v, 1, strdup("Bastardo"));
  assert(vector_getsize(v) == 4);
  assert(strcmp(vector_getat(v, 1), "Bastardo") == 0);
  vector_print(v);
  vector_destroy_deep(v);
  print_testpass();
}

void test_insert_tr() {
  print_testheader("Insert (Tail Recursive)");
  vector v = vector_create();
  vector_append(v, strdup("Dio"));
  vector_append(v, strdup("Cane"));
  vector_append(v, strdup("Mostruoso"));
  vector_print(v);
  vector_insert_tr(v, 1, strdup("Bastardo"));
  assert(vector_getsize(v) == 4);
  assert(strcmp(vector_getat(v, 1), "Bastardo") == 0);
  vector_print(v);
  vector_destroy_deep(v);
  print_testpass();
}

void test_grow() {
  print_testheader("Grow");
  vector v = vector_create();
  vector_print(v);
  assert(vector_getcapacity(v) == 4);
  vector_append(v, strdup("Dio"));
  vector_append(v, strdup("Cane"));
  vector_append(v, strdup("Dio"));
  vector_append(v, strdup("Mostruoso"));
  vector_append(v, strdup("Gesù"));
  vector_append(v, strdup("Merdoso"));
  vector_append(v, strdup("Gesù"));
  vector_append(v, strdup("Ladro"));
  vector_append(v, strdup("Madonna"));
  vector_append(v, strdup("Vergognosa"));
  vector_append(v, strdup("Madonna"));
  vector_append(v, strdup("Malata"));
  assert(vector_getcapacity(v) == 16);
  vector_print(v);
  print_testpass();
}

void test_delete(){
  print_testheader("Delete");
  vector v = vector_create();
  vector_append(v, strdup("Dio"));
  vector_append(v, strdup("Cane"));
  vector_append(v, strdup("Mostruoso"));
  assert(vector_getsize(v) == 3);
  vector_print(v);
  vector_delete(v, 1);
  vector_print(v);
  assert(strcmp(vector_getat(v, 0), "Dio") == 0);
  assert(strcmp(vector_getat(v, 1), "Mostruoso") == 0);
  assert(vector_getsize(v) == 2); 
  vector_destroy_deep(v);
  print_testpass();
}

void test_delete_opt() {
  print_testheader("Delete (Optimized)");
  vector v = vector_create();
  vector_append(v, strdup("Dio"));
  vector_append(v, strdup("Cane"));
  vector_append(v, strdup("Mostruoso"));
  assert(vector_getsize(v) == 3);
  vector_print(v);
  vector_delete_opt(v, 1);
  vector_print(v);
  assert(strcmp(vector_getat(v, 0), "Dio") == 0);
  assert(strcmp(vector_getat(v, 1), "Mostruoso") == 0);
  assert(vector_getsize(v) == 2); 
  vector_destroy_deep(v);
  print_testpass();
}

void test_delete_tr(){
  print_testheader("Delete (Tail Recursive)");
  vector v = vector_create();
  vector_append(v, strdup("Dio"));
  vector_append(v, strdup("Cane"));
  vector_append(v, strdup("Mostruoso"));
  assert(vector_getsize(v) == 3);
  vector_print(v);
  vector_delete_tr(v, 1);
  vector_print(v);
  assert(strcmp(vector_getat(v, 0), "Dio") == 0);
  assert(strcmp(vector_getat(v, 1), "Mostruoso") == 0);
  assert(vector_getsize(v) == 2); 
  vector_destroy_deep(v);
  print_testpass();
}

void test_remove(){
  print_testheader("Remove");
  vector v = vector_create();
  vector_append(v, strdup("Dio"));
  vector_append(v, strdup("Cane"));
  vector_append(v, strdup("Mostruoso"));
  assert(vector_getsize(v) == 3);
  vector_print(v);
  char * s = vector_remove(v, 1);
  vector_print(v);
  assert(strcmp(vector_getat(v, 0), "Dio") == 0);
  assert(strcmp(vector_getat(v, 1), "Mostruoso") == 0);
  assert(vector_getsize(v) == 2); 
  printf("Removed: [%s]\n", s);
  vector_destroy_deep(v);
  print_testpass();
}

void test_remove_tr(){
}

void test_delete_first(){
  print_testheader("Delete First");
  vector v = vector_create();
  vector_append(v, strdup("Dio"));
  vector_append(v, strdup("Cane"));
  vector_append(v, strdup("Mostruoso"));
  assert(vector_getsize(v) == 3);
  vector_print(v);
  vector_delete_first(v);
  vector_print(v);
  assert(strcmp(vector_getat(v, 0), "Cane") == 0);
  assert(strcmp(vector_getat(v, 1), "Mostruoso") == 0);
  assert(vector_getsize(v) == 2); 
  vector_destroy_deep(v);
  print_testpass();
}

void test_delete_first_tr(){
}

void test_remove_first(){
  print_testheader("Remove First");
  vector v = vector_create();
  vector_append(v, strdup("Dio"));
  vector_append(v, strdup("Cane"));
  vector_append(v, strdup("Mostruoso"));
  assert(vector_getsize(v) == 3);
  vector_print(v);
  char * s = vector_remove_first(v);
  vector_print(v);
  assert(strcmp(vector_getat(v, 0), "Cane") == 0);
  assert(strcmp(vector_getat(v, 1), "Mostruoso") == 0);
  assert(vector_getsize(v) == 2); 
  printf("Removed: [%s]\n", s);
  vector_destroy_deep(v);
  print_testpass();
}

void test_remove_first_tr(){
}

void test_delete_last() {
  print_testheader("Delete Last");
  vector v = vector_create();
  vector_append(v, strdup("Dio"));
  vector_append(v, strdup("Cane"));
  vector_append(v, strdup("Mostruoso"));
  assert(vector_getsize(v) == 3);
  vector_print(v);
  vector_delete_last(v);
  vector_print(v);
  assert(strcmp(vector_getat(v, 0), "Dio") == 0);
  assert(strcmp(vector_getat(v, 1), "Cane") == 0);
  assert(vector_getsize(v) == 2); 
  vector_destroy_deep(v);
  print_testpass();
}

void test_delete_last_tr(){
}

void test_remove_last(){
  print_testheader("Remove Last");
  vector v = vector_create();
  vector_append(v, strdup("Dio"));
  vector_append(v, strdup("Cane"));
  vector_append(v, strdup("Mostruoso"));
  assert(vector_getsize(v) == 3);
  vector_print(v);
  char * s = vector_remove_last(v);
  vector_print(v);
  assert(strcmp(vector_getat(v, 0), "Dio") == 0);
  assert(strcmp(vector_getat(v, 1), "Cane") == 0);
  assert(vector_getsize(v) == 2); 
  printf("Removed: [%s]\n", s);
  vector_destroy_deep(v);
  print_testpass();

}

void test_remove_last_tr(){
}

void test_delete_all(){
  print_testheader("Delete All");
  vector v = vector_create();
  vector_append(v, strdup("Dio"));
  vector_append(v, strdup("Cane"));
  vector_append(v, strdup("Gesù"));
  vector_append(v, strdup("Mostruoso"));
  vector_append(v, strdup("Gesù"));
  vector_append(v, strdup("Dio"));
  vector_append(v, strdup("Cane"));
  vector_append(v, strdup("Animale"));
  assert(vector_getsize(v) == 8);
  vector_print(v);
  vector_delete_all(v, "Dio");
  vector_delete_all(v, "Cane");
  vector_print(v);
  assert(vector_search(v, "Dio") == -1);
  assert(vector_search(v, "Cane") == -1);
  assert(vector_getsize(v) == 4); 
  vector_destroy_deep(v);
  print_testpass();
}

void test_delete_all_tr(){
}

void test_search(){
  print_testheader("Search");
  vector v = vector_create();
  vector_append(v, strdup("Madonna"));
  vector_append(v, strdup("Troia"));
  vector_append(v, strdup("Laida"));
  assert(vector_getsize(v) == 3);
  vector_print(v);
  printf("Searching for: [%s].\n", vector_getat(v, 1));
  vector_size_t p = (vector_size_t) vector_search(v, "Troia");
  assert(p == 1);
  printf("Found at: [%ld].\n", p);
  printf("Searching for: [%s].\n", "Puttana");
  int q = vector_search(v, "Puttana");
  assert(q == -1);
  printf("Not Found (%d).\n", q);
  print_testpass();
}

void test_search_tr(){
}

void test_all(){
  test_create_destroy();
  test_print_tr();

  test_prepend();
  test_prepend_opt();
  test_prepend_tr();

  test_append();

  test_insert();
  test_insert_opt();
  test_insert_tr();

  test_grow();

  test_delete();
  test_delete_opt();
  test_delete_tr();

  test_remove();
  test_remove_tr();

  test_delete_first();
  test_delete_first_tr();

  test_remove_first();
  test_remove_first_tr();

  test_delete_last();
  test_delete_last_tr();

  test_remove_last();
  test_remove_last_tr();

  test_delete_all();
  test_delete_all_tr();

  test_search();
  test_search_tr();
}

int main(int argc, char *argv[]) {
  if (argc < MIN_ARGS) {
    fprintf(stderr, "error: testunit: missing arguments: "
            "specify the name of the functions you wish to test.\n");
    exit(1);
  }
  switch (argv[1][0]) {
    case 'c':
    case 'C':
      test_create_destroy();
      test_print_tr();
      break;

    case 'i':
    case 'I':
      test_grow();

      test_prepend();
      test_prepend_opt();
      test_prepend_tr();

      test_append();

      test_insert();
      test_insert_opt();
      test_insert_tr();
      break;

    case 'd':
    case 'D':
      test_delete();
      test_delete_opt();
      test_delete_tr();

      test_remove();

      test_delete_first();

      test_remove_first();

      test_delete_last();

      test_remove_last();

      test_delete_all();
      break;

    case 's':
    case 'S':
      test_search();
      break;

    default:
      test_all();
      break;
  }
}

/* end of testunit.c */
