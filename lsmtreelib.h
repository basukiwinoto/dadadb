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
int sstable_count(int level);
int flush();
int sort(struct keyval **tree, int lo, int hi);
int partition(struct keyval **tree, int lo, int hi);
int merge(struct keyval **tree, int len, int level);
int merge2(int l1, int l2);
int print_merged_content(int level);
int print_lsm_tree();
int get_from_sstable(int k, int level);
int load(char *target);
int stat();

#endif