#include <stdio.h>
#include <stdlib.h>
#include "lsmtreelib.h"


struct keyval {
	struct keyval *prev;
	int key;
	int value;
	char flag;
};

struct keyval *last = '\0';


struct keyval *next(struct keyval *current){
	return current->prev;
}

int put(int k, int v){
	struct keyval *kv = (struct keyval *) malloc(sizeof(struct keyval));
	kv->prev = last;
	kv->key = k;
	kv->value = v;
	kv->flag = 'i';
	last = kv;
	return 0;
}

int display(struct keyval *current){
	struct keyval *kv = current;
	if(kv=='\0'){
		perror("List is empty");
		return -1;
	}
	while(kv != '\0'){
		if (kv->flag != 'd'){
			printf("%d:%d ", kv->key, kv->value);
		}
		kv = next(kv);
	}
	return 0;
}

int display_all(){
	return display(last);
}

int get(int k){
	struct keyval *kv = last;
	if(kv=='\0'){
		perror("List is empty");
		return -1;
	}
	while(kv != '\0'){
		if ((kv->key == k) && (kv->flag != 'd')) {
			return kv->value;
		}
		kv = next(kv);
	}
	return -1;
}

int delete(int k){
	struct keyval *kv = last;
	int delkey = -1;
	if(kv=='\0'){
		perror("List is empty");
		return -1;
	}
	while(kv != '\0'){
		if (kv->key == k) {
			kv->flag='d';
			delkey = k;
		}
		kv = next(kv);
	}
	return delkey;
}

struct keyval *range(int start, int end){
	struct keyval *kv = last;
	struct keyval *ret = '\0';
	if(kv=='\0'){
		perror("List is empty");
		return NULL;
	}
	while(kv != '\0'){
		if ((kv->key >= start) && (kv->key < end)
			&& (kv->flag != 'd') && (keyexists(kv->key, ret) == 0)) {
			struct keyval *r = (struct keyval *) malloc(sizeof(struct keyval));
			r->prev = ret;
			r->key = kv->key;
			r->value = kv->value;
			r->flag = kv->flag;
			ret = r;
		}
		kv = next(kv);
	}
	return ret;	
}

int keyexists(int k, struct keyval *current){
	while(current != '\0'){
		if(current->key == k){
			return 1;
		}
		current = next(current);
	}
	return 0;
}

int count(){
	struct keyval *kv = last;
	int c = 0;
	while(kv != '\0'){
		if ((kv->flag != 'd') && (keyexists(kv->key, next(kv)) == 0)) {
			c++;
		}
		kv = next(kv);
	}
	return c;	
}