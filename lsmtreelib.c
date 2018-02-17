#include <stdio.h>
#include <stdlib.h>
#include "lsmtreelib.h"

#define MAX_LSMTREE_LEN 1000

struct keyval {
	int key;
	int value;
	char flag;
};

struct keyval *lsmtree[MAX_LSMTREE_LEN];
int lsmtidx=0;

int put(int k, int v){
	struct keyval *kv = (struct keyval *) malloc(sizeof(struct keyval));
	kv->key = k;
	kv->value = v;
	kv->flag = 'i';
	lsmtree[lsmtidx] = kv;
	lsmtidx++;
	return 0;
}

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
	if(kv->flag=='d'){
		return -1;
	}
	return kv->value;
}

int delete(int k){
	struct keyval *kv = (struct keyval *) malloc(sizeof(struct keyval));
	kv->key = k;
	kv->flag = 'd';
	lsmtree[lsmtidx] = kv;
	lsmtidx++;
	return 0;
}

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

int display(){
	return display_lsmtree(lsmtree, lsmtidx);
}

int display_lsmtree(struct keyval **lsmt, int len){
	struct keyval *kv;

	int i;
	for(i=len-1;i>=0;i--){
		kv = lsmt[i];
		if(kv->flag!='d' && is_latest(kv, lsmt, len)){
			printf("%d:%d ", kv->key, kv->value);
		}
	}
	return len;
}

int is_latest(struct keyval *kv, struct keyval **lsmt, int len){
	int i;
	for(i=len-1;i>=0;i--){
		if(lsmt[i]->key == kv->key){
			break;
		}
	}
	
	return kv==lsmt[i];
}

int count(){
	return lsmtidx;	
}
