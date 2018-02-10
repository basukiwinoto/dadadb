/***
 * dadadb is a small data system that store key value pair in a lsmtree format
 * author: Basuki Winoto
 **/
#include <stdio.h>
#include <stdlib.h>

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
	int loop = 1;
	
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
			perror("do nothing. unknown command");
			break;
	}
}

void proc_p(char *cmd){
	printf("put command : ");
	printf("%s",cmd);
}

void proc_g(char *cmd){
	printf("get command : ");
	printf("%s",cmd);
}

void proc_r(char *cmd){
	printf("range command : ");
	printf("%s",cmd);
}

void proc_d(char *cmd){
	printf("delete command : ");
	printf("%s",cmd);
}

void proc_l(char *cmd){
	printf("load command : ");
	printf("%s",cmd);
}

void proc_s(char *cmd){
	printf("stat command : ");
	printf("%s",cmd);
	printf("Total Pairs: %d\n", 12);
}
