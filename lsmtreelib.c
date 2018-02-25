/***
 * lsmtreelib.c implements an lsm-tree structure and operations
 * author: Basuki Winoto
 **/
#include <stdio.h>
#include <stdlib.h>
#include "lsmtreelib.h"
#include <stddef.h>

/***
 * macros
 **/
#define MAX_MEMTABLE_LEN	4		/* max buffer size*/
#define FLAG_INSERTED		'i'		/* flag inserted */
#define	FLAG_DELETED		'd'		/* flag deleted */
#define MAX_SSTABLE_LEN		10 		/* max sstable levels */

/***
 * struct keyval holds a key-value pair, with flag for deletion
 **/
struct keyval {
	int k;	/* key */
	int v;	/* value */
	char f;	/* flag - FLAG_INSERTED, FLAG_DELETED*/
	int i;	/* insertion order */
};

/***
 * memtable is the C0 memory buffer
 * memtable_idx is the index of the next empty in the buffer
 **/
struct keyval *memtable[MAX_MEMTABLE_LEN];
int memtable_idx = 0;
int sstable_max[MAX_SSTABLE_LEN] = {0, 2, 2, 4, 4, 0, 2, 2, 4, 4};
int sstable_idx[MAX_SSTABLE_LEN] = {0};

/***
 * put inserts a new pair of key k and value v to the lsm-tree
 **/
int put(int k, int v){
	int i = append(k, v, FLAG_INSERTED);

	if (i == MAX_MEMTABLE_LEN){
		flush();
	}
		
	return i;
}

/***
 * get reads the most recent non-deleted value from the lsm-tree
 **/
int get(int k){
	struct keyval *kv;
	int i;
	
	for(i=memtable_idx;i>0;i--){
		kv = memtable[i-1];
		if (kv->k == k)
			break;
	}
	if(i==0) 				/* not found */
		return get_from_sstable(k, 1);
	if(kv->f==FLAG_DELETED)	/* deleted */
		return -1;
	
	return kv->v;
}

/***
 * delete inserts a new pair of key k with flag deleted to the lsm-tree
 **/
int delete(int k){
	int i = append(k, 0, FLAG_DELETED);

	if (i == MAX_MEMTABLE_LEN)
		flush();
	
	return i;
}

/***
 * range gets the key value pairs with key between lo and hi
 **/
int *range(int lo, int hi){
	int *r = (int *)malloc(sizeof(int)*(hi-lo));
	for(int i=lo;i<hi;i++){
		r[i-lo] = get(i);
	}
	return r;
}

/***
 * append creates key-value pair and adds it to the buffer
 **/
int append(int k, int v, char f){
	/* append to C0 */
	struct keyval *kv = (struct keyval *) malloc(sizeof(struct keyval));

	kv->k = k;
	kv->v = v;
	kv->f = f;
	kv->i = memtable_idx;
	memtable[memtable_idx] = kv;
	memtable_idx++;

	return memtable_idx;
}

/***
 * count returns the length of the lsm-tree
 **/
int count(){
	int pairs = 0;
	int i;
	for(i=0;i<MAX_SSTABLE_LEN;i++){
		pairs += sstable_idx[i];
	}
	return pairs + memtable_idx;
}

int sstable_count(int level){
	return sstable_idx[level];
}

/***
 * flush sorts the buffer and merge to the higher level
 **/
int flush(){
	sort(memtable, 0, memtable_idx-1);
	merge(memtable, memtable_idx, 1);
	memtable_idx = 0;
	return 0;
}

int sort(struct keyval **tree, int lo, int hi){
	if(lo<hi){
		int p = partition(tree, lo, hi);
		sort(tree, lo, p-1);
		sort(tree, p+1, hi);
	}
	return 0;
}

int partition(struct keyval **tree, int lo, int hi){
	struct keyval *pivot = tree[hi];
	struct keyval *tmp;
	int i=lo-1;
	int j;
	for(j=lo; j<hi; j++){
		if((tree[j]->k > pivot->k) ||
		   (tree[j]->k == pivot->k && tree[j]->i < pivot->i)){
			i++;
			tmp = tree[i];
			tree[i] = tree[j];
			tree[j] = tmp;
		}
	}
	tmp = tree[i+1];
	tree[i+1] = tree[hi];
	tree[hi] = tmp;
	return i+1;
}

int merge(struct keyval **tree, int len, int level){
	char target[256];
	snprintf(target, 256, "c%d.sstable", level);

	struct keyval *kv;
	struct keyval *last = NULL;
	
	int i, c=0;
	
	FILE *ftgt = fopen(target,"rb");
	FILE *ftmp = fopen("tmpfile","wb");
	int k,v;
		
	if(!ftgt){ /* target is empty, copy only tree */
		for(i=len;i>0;i--){
			kv = tree[i-1];
			if ((!last || kv->k != last->k) 
				&& kv->f != FLAG_DELETED){ /* skip duplicates and deleted */
				/* write kv */
				fwrite((const void *)&kv->k, sizeof(int), 1, ftmp);
				fwrite((const void *)&kv->v, sizeof(int), 1, ftmp);
				c++;
			}
			last=kv;
		}
	} else { /* target not empty, merge tree */
		i=len-1;
		kv=tree[i];
		/* adv k */
		fread(&k, sizeof(int), 1, ftgt);
		fread(&v, sizeof(int), 1, ftgt);
		while(i>=0 && !feof(ftgt)){
			if(k == kv->k){
				/* write kv */
				if(kv->f != FLAG_DELETED){
					fwrite((const void *)&kv->k, sizeof(int), 1, ftmp);
					fwrite((const void *)&kv->v, sizeof(int), 1, ftmp);
					c++;
				}
				/* adv k */
				fread(&k, sizeof(int), 1, ftgt);
				fread(&v, sizeof(int), 1, ftgt);
				/* adv kv */
				last = kv;
				while(i>=0 && kv->k == last->k){
					i--;
					kv = tree[i];
				}
			} else if (k < kv->k) {
				/* write k */
				fwrite((const void *)&k, sizeof(int), 1, ftmp);
				fwrite((const void *)&v, sizeof(int), 1, ftmp);
				c++;
				/* adv k */
				if(fread(&k, sizeof(int), 1, ftgt) != 1 ||
					fread(&v, sizeof(int), 1, ftgt) != 1)
					break;
			} else if (k > kv->k) {
				/* write kv */
				if(kv->f != FLAG_DELETED){
					fwrite((const void *)&kv->k, sizeof(int), 1, ftmp);
					fwrite((const void *)&kv->v, sizeof(int), 1, ftmp);
					c++;
				}
				/* adv kv */
				last = kv;
				while(i>=0 && kv->k == last->k){
					i--;
					kv = tree[i];
				}
			}
		}
	
		
	}
	
	if (ftgt)
		fclose(ftgt);
	if (ftmp)
		fclose(ftmp);
	rename("tmpfile", target);
	sstable_idx[1] = c;
	if (sstable_idx[1]>=sstable_max[1])
		merge2(1, 2);
	return 0;
}

int merge2(int l1, int l2){
	printf("merging l1 to l2\n");
	if (sstable_idx[l2]>=sstable_max[l2])
		merge2(l2,l2+1);
	return l1+l2;
	
}

int print_merged_content(int level){
	char target[256];
	snprintf(target, 256, "c%d.sstable", level);
	
	FILE *ftgt = fopen(target,"rb");
	if(!ftgt) /* file not exists */
		return 0;
	
	printf("%s:\n", target);
	int k, v;
	while(!feof(ftgt)){
		if(fread(&k, sizeof(int), 1, ftgt) &&
			fread(&v, sizeof(int), 1, ftgt))
			printf("%d:%d:L%d ", k, v,level);
	}
	printf("\n");
	fclose(ftgt);
	return 0;
}

int print_lsm_tree(){
	printf("c0:\n");
	for(int i=0;i<memtable_idx;i++){
		printf("%d:%d ", memtable[i]->k, memtable[i]->v);
	}
	printf("\n");
	for(int i=1;i<MAX_SSTABLE_LEN;i++){
		if (sstable_idx[i]>0)
			print_merged_content(i);
	}
	printf("\n");
	return 0;
}

int get_from_sstable(int k, int level){
	char target[256];
	snprintf(target, 256, "c%d.sstable", level);
	
	FILE *ftgt = fopen(target,"rb");
	if(!ftgt) /* file not exists */
		return -1;
	
	int key, value;
	while(!feof(ftgt)){
		if(fread(&key, sizeof(int), 1, ftgt) &&
			fread(&value, sizeof(int), 1, ftgt) && key == k)
			return value;
	}
	return -1;	
}

int load(char *target){
	FILE *ftgt = fopen(target,"rb");
	if(!ftgt)	/* file not exists */
		return 0;
	
	int k, v, i=0;
	while(!feof(ftgt)){
		if(fread(&k, sizeof(int), 1, ftgt) &&
			fread(&v, sizeof(int), 1, ftgt)){
			put(k,v);
			i++;
		}
	}
	fclose(ftgt);
	return i;
}

int stat(){
	printf("Total Pairs: %d\n", count());
	for(int i=1;i<MAX_SSTABLE_LEN;i++){
		if (sstable_idx[i]>0)
			printf("LVL%d: %d, ", i, sstable_count(i));
	}
	printf("\n");
	print_lsm_tree();
	return 0;
}