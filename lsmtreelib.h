#ifndef LSMTREELIB_H
#define LSMTREELIB_H
/***
 * lsmtreelib.h - header file with declaration functions in lsmtree.c
 **/
struct keyval;

int append(int k, int v, char f);
int put(int k, int v);
int get(int k);
int delete(int k);
int *range(int start, int end);
int count();
int flush();
int sort(struct keyval **tree, int lo, int hi);
int partition(struct keyval **tree, int lo, int hi);
int merge(struct keyval **tree, int len, char *target);
int print_merged_content(char *target);

#endif