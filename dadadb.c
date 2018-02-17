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
	//uncomment to debug command
	printf("cmd> %s key: %d value: %d\n", cmd, i1, i2);
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
	//uncomment to debug command
	printf("cmd> %s key: %d\n", cmd, i1);
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
	//uncomment to debug the command
	printf("cmd> %s key: %d value: %d\n", cmd, i1, i2);
	struct keyval *lsmt[count()];
	int len = range(i1,i2,lsmt);
	display_lsmtree(lsmt, len);
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
	//uncomment to debug command
	printf("cmd> %s key: %d\n", cmd, i1);
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
	printf("cmd> %s file: %s", cmd, tok);
}

void proc_s(char *cmd){
	printf("cmd> %s",cmd);
	printf("Total Pairs: %d\n", count());
	display();
	printf("\n");
}
