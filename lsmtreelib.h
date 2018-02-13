#ifndef LSMTREELIB_H
#define LSMTREELIB_H
/***
 * lsmtreelib.h - header file with declaration functions in lsmtree.c
 **/

struct keyval *next(struct keyval *current);
int put(int k, int v);
int display(struct keyval *current);
int display_all();
int get(int k);
int delete(int k);
struct keyval *range(int start, int end);
int keyexists(int k, struct keyval *current);
int count();


#endif