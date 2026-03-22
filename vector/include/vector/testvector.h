
/* ===========================| --------------- |=============================
 * Test Vector
 *
 * This header file provides declarations of test functions used to check for 
 * correctness and measure performance of the dynamic vector algorithms 
 * implemented in 'vector.c'.
 *
 * ===========================| --------------- |=============================*/
#ifndef TESTVECTOR_H
#define TESTVECTOR_H

void test_create_destroy();
void test_print_tr();

void test_prepend();
void test_prepend_tr();
void test_prepend_opt();

void test_append();

void test_insert();
void test_insert_tr();

void test_grow();
void test_getbytes();

void test_delete();
void test_delete_tr();

void test_remove();
void test_remove_tr();

void test_delete_first();
void test_delete_first_tr();

void test_remove_first();
void test_remove_first_tr();

void test_delete_last();
void test_delete_last_tr();

void test_remove_last();
void test_remove_last_tr();

void test_delete_all();
void test_delete_all_tr();

void test_search();
void test_search_tr();

void test_all();

#endif
/* end of testvector.h */
