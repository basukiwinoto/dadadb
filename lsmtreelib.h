#ifndef LSMTREELIB_H
#define LSMTREELIB_H
/***
 * lsmtreelib.h - header file with declaration functions in lsmtree.c
 **/
struct keyval;

int put(int k, int v);
int get(int k);
int delete(int k);
int range(int start, int end, struct keyval **rlsmt);
int display();
int display_lsmtree(struct keyval **lsmt, int len);
int is_latest(struct keyval *kv, struct keyval **lsmt, int len);
int count();


#endif