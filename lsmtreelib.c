/***
 * lsmtreelib.c implements an lsm-tree structure and operations
 * author: Basuki Winoto
 **/
#include <stdio.h>
#include <stdlib.h>
#include "lsmtreelib.h"

/***
 * macros
 **/
#define MAX_LSMTREE_LEN	100	/* max buffer size*/
#define FLAG_INSERTED	'i'		/* flag inserted */
#define	FLAG_DELETED	'd'		/* flag deleted */

/***
 * struct keyval holds a key-value pair, with flag for deletion
 **/
struct keyval {
	int key;
	int value;
	char flag;
	int insidx;
};

/***
 * lsmtree is the C0 memory buffer
 * lsmtidx is the index of the next empty in the buffer
 **/
struct keyval *lsmtree[MAX_LSMTREE_LEN];
int lsmtidx=0;

/***
 * append creates key-value pair and adds it to the buffer
 **/
int append(int k, int v, char f){
	struct keyval *kv = (struct keyval *) malloc(sizeof(struct keyval));
	kv->key = k;
	kv->value = v;
	kv->flag = f;
	kv->insidx = lsmtidx;
	lsmtree[lsmtidx] = kv;
	lsmtidx++;
	if(lsmtidx==MAX_LSMTREE_LEN){ /* merging if buffer is full */
		merge(lsmtree, lsmtidx, "C1_merged");
		print_merged_content("C1_merged");
		flush();
	}
	return lsmtidx;
}

/***
 * put inserts a new pair of key k and value v to the lsm-tree
 **/
int put(int k, int v){
	return append(k, v, FLAG_INSERTED);
}

/***
 * get reads the most recent non-deleted value from the lsm-tree
 **/
int get(int k){
	struct keyval *kv;
	int i;
	for(i=lsmtidx-1;i>=0;i--){
		kv = lsmtree[i];
		if (kv->key==k){
			break;
		}
	}
	if(i==0){
		return -1;
	}
	if(kv->flag==FLAG_DELETED){
		return -1;
	}
	return kv->value;
}

/***
 * delete inserts a new pair of key k with flag deleted to the lsm-tree
 **/
int delete(int k){
	return append(k, 0, FLAG_DELETED);
}

/***
 * range gets the key value pairs with key between start and end
 **/
int range(int start, int end, struct keyval **rlsmt){
	int rlsmtidx = 0;

	struct keyval *kv;
	int i;
	for(i=0;i<lsmtidx;i++){
		kv = lsmtree[i];
		if ((kv->key>=start) && (kv->key < end)){
			rlsmt[rlsmtidx] = kv;
			rlsmtidx++;
		}
	}
	return rlsmtidx;
}

/***
 * display prints out the lsm-tree
 **/
int display(){
	sort(lsmtree,0,lsmtidx-1);
	display_lsmtree(lsmtree, lsmtidx);
	//merge(lsmtree, lsmtidx, "C1_merged");
	//print_merged_content("C1_merged");
	return 0;
}

/***
 * display_lsmtree prints out a lsm-tree lsmt with length of len
 **/
int display_lsmtree(struct keyval **lsmt, int len){
	struct keyval *kv;

	int i;
	for(i=len-1;i>=0;i--){
		kv = lsmt[i];
		if(kv->flag!=FLAG_DELETED && is_latest(kv, lsmt, len)){
			printf("%d:%d ", kv->key, kv->value);
		}
	}
	return len;
}

/***
 * is_latest tests if a key-value pair kv is the most recent in the lsmt
 **/
int is_latest(struct keyval *kv, struct keyval **lsmt, int len){
	int i;
	for(i=len-1;i>=0;i--){
		if(lsmt[i]->key == kv->key){
			break;
		}
	}
	
	return kv==lsmt[i];
}

/***
 * count returns the length of the lsm-tree
 **/
int count(){
	return lsmtidx;	
}

/***
 * flush sorts the buffer and merge to the higher level
 **/
int flush(){
	lsmtidx=0;
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
		if((tree[j]->key > pivot->key) ||
		   (tree[j]->key == pivot->key && tree[j]->insidx < pivot->insidx)){
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

int merge(struct keyval **tree, int len, char *target){
	struct keyval *kv;
	struct keyval *last = NULL;
	
	int i;
	
	FILE *ftgt = fopen(target,"rb");
	FILE *ftmp = fopen("tmpfile","wb");
	int k,v;
		
	if(!ftgt){ /* target is empty, copy only tree */
		for(i=len-1;i>=0;i--){
			kv = tree[i];
			if ((!last || kv->key != last->key) 
				&& kv->flag != FLAG_DELETED){ /* skip duplicates and deleted */
				/* write kv */
				fwrite((const void *)&kv->key, sizeof(int), 1, ftmp);
				fwrite((const void *)&kv->value, sizeof(int), 1, ftmp);
			}
			last=kv;
		}
	} else { /* target not empty, merge tree */
		i=len-1;
		kv=tree[i];
		fread(&k, sizeof(int), 1, ftgt);
		fread(&v, sizeof(int), 1, ftgt);
		while(i>=0 && !feof(ftgt)){
			printf("f %d : t %d", k, kv->key);
			if(k == kv->key){
				printf("=\n");
				/* write kv */
				if(kv->flag != FLAG_DELETED){
					fwrite((const void *)&kv->key, sizeof(int), 1, ftmp);
					fwrite((const void *)&kv->value, sizeof(int), 1, ftmp);
				}
				/* adv k */
				fread(&k, sizeof(int), 1, ftgt);
				fread(&v, sizeof(int), 1, ftgt);
				/* adv kv */
				last = kv;
				while(i>=0 && kv->key == last->key){
					i--;
					kv = tree[i];
				}
			} else if (k < kv->key) {
				printf("<\n");
				/* write k */
				fwrite((const void *)&k, sizeof(int), 1, ftmp);
				fwrite((const void *)&v, sizeof(int), 1, ftmp);
				/* adv k */
				if(fread(&k, sizeof(int), 1, ftgt) != 1 ||
					fread(&v, sizeof(int), 1, ftgt) != 1)
					break;
			} else if (k > kv->key) {
				printf(">\n");
				/* write kv */
				if(kv->flag != FLAG_DELETED){
					fwrite((const void *)&kv->key, sizeof(int), 1, ftmp);
					fwrite((const void *)&kv->value, sizeof(int), 1, ftmp);
				}
				/* adv kv */
				last = kv;
				while(i>=0 && kv->key == last->key){
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
	return 0;
}

int print_merged_content(char *target){
	FILE *ftgt = fopen(target,"rb");
	int k, v;
	while(!feof(ftgt)){
		fread(&k, sizeof(int), 1, ftgt);
		fread(&v, sizeof(int), 1, ftgt);
		printf("%d:%d\n", k, v);
	}
	
	fclose(ftgt);
	return 0;
}