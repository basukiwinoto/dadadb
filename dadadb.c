/***
 * dadadb is a small data system that store key value pair in a lsmtree format
 * author: Basuki Winoto
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lsmtreelib.h"

/***
 * macros
 **/
#define MAX_CMD_LEN 256			/* maximum length of a command */
#define VERBOSE 0		/* verbose mode 1 - on, 0 - off */

/***
 * prototypes
 **/
void proc_cmd(char *cmd);
void proc_p(char *cmd);
void proc_g(char *cmd);
void proc_r(char *cmd);
void proc_d(char *cmd);
void proc_l(char *cmd);
void proc_s(char *cmd);

/***
 * main function
 **/
int main(int argc, char **argv){
	FILE *f;
	char *cmd;
	
	if(argc==1){						/* exit if no arg */
		perror("Missing argument file input");
		return -1;
	}
	
	if((f=fopen(argv[1],"r")) == NULL){ /* exit if can't open file */
		perror("Cannot open file");
		return -1;
	}
	
	
	while( (cmd = fgets(cmd, MAX_CMD_LEN, f)) != NULL ){	/* read lines */
		proc_cmd(cmd);
	}
	
	//uncomment to debug list content
	//display_all();
	return 0;
}

/***
 * function proc_cmd parses and executes command cmd according to the domain
 * specific language.
 * return value : -
 **/
void proc_cmd(char *cmd){
	switch(cmd[0]){
		case 'p':
			proc_p(cmd);
			break;
		case 'g':
			proc_g(cmd);
			break;
		case 'r':
			proc_r(cmd);
			break;
		case 'd':
			proc_d(cmd);
			break;
		case 'l':
			proc_l(cmd);
			break;
		case 's':
			proc_s(cmd);
			break;
		default:
			perror("Do nothing. Unknown command");
			break;
	}
}

void proc_p(char *cmd){
	char *tok;
	int i1, i2;
	if((tok=strtok(cmd, " "))==NULL){
		perror("Do nothing. Malformed command.");
		return;
	}
	if((tok=strtok(NULL, " "))==NULL){
		perror("Do nothing. Malformed command.");
		return;
	}
	i1 = strtol(tok, NULL, 10);
	if((tok=strtok(NULL, " "))==NULL){
		perror("Do nothing. Malformed command.");
		return;
	}
	i2 = strtol(tok, NULL, 10);	
	if(VERBOSE){
		printf("cmd> %s key: %d value: %d\n", cmd, i1, i2);
		print_lsm_tree();
	}
	put(i1,i2);
}

void proc_g(char *cmd){
	char *tok;
	int i1;
	if((tok=strtok(cmd, " "))==NULL){
		perror("Do nothing. Malformed command.");
		return;
	}
	if((tok=strtok(NULL, " "))==NULL){
		perror("Do nothing. Malformed command.");
		return;
	}
	i1 = strtol(tok, NULL, 10);
	if(VERBOSE){
		printf("cmd> %s key: %d\n", cmd, i1);
		print_lsm_tree();
	}
	int v = get(i1);
	if(v!=-1){
		printf("%d",get(i1));
	}
	printf("\n");
}

void proc_r(char *cmd){
	char *tok;
	int i1, i2;
	if((tok=strtok(cmd, " "))==NULL){
		perror("Do nothing. Malformed command.");
		return;
	}
	if((tok=strtok(NULL, " "))==NULL){
		perror("Do nothing. Malformed command.");
		return;
	}
	i1 = strtol(tok, NULL, 10);
	if((tok=strtok(NULL, " "))==NULL){
		perror("Do nothing. Malformed command.");
		return;
	}
	i2 = strtol(tok, NULL, 10);	
	if(VERBOSE){
		printf("cmd> %s key: %d value: %d\n", cmd, i1, i2);
		print_lsm_tree();
	}
	int	*r = range(i1,i2);
	for(int i=i1;i<i2;i++) {
		if(r[i-i1]>-1)
			printf("%d:%d ", i, r[i-i1]);
	}
	printf("\n");
}

void proc_d(char *cmd){
	char *tok;
	int i1;
	if((tok=strtok(cmd, " "))==NULL){
		perror("Do nothing. Malformed command.");
		return;
	}
	if((tok=strtok(NULL, " "))==NULL){
		perror("Do nothing. Malformed command.");
		return;
	}
	i1 = strtol(tok, NULL, 10);
	if(VERBOSE){
		printf("cmd> %s key: %d\n", cmd, i1);
		print_lsm_tree();
	}
	delete(i1);
}

void proc_l(char *cmd){
	char *tok;
	if((tok=strtok(cmd, " "))==NULL){
		perror("Do nothing. Malformed command.");
		return;
	}
	if((tok=strtok(NULL, " "))==NULL){
		perror("Do nothing. Malformed command.");
		return;
	}
	if(VERBOSE){
		printf("cmd> %s file: %s", cmd, tok);
		print_lsm_tree();
	}
	load(tok);
}

void proc_s(char *cmd){
	if(VERBOSE){
		printf("cmd> %s",cmd);
		print_lsm_tree();
	}
	stat();
}
