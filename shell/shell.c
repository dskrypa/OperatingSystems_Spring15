/**
 * Douglas Skrypa
 * Assignment 1: Shell
 * 2015.02.19
 */

#include <stdio.h>					//IO functions
#include <stdlib.h>					//Memory functions
#include <string.h>					//String functions
#include <ctype.h>					//Character conversions and testing
#include <unistd.h>

#include "LinkedList.h"
#include "shell.h"

int main(int argc, char** argv){
	printf("DS Shell\n");
	unsigned canRun = 1, c = 0;
	List h = list_create(10);
	while(canRun) {
		char wd[2048];
		if (getcwd(wd, sizeof(wd)) != NULL) {
			printf("%s » ", wd);
		} else {
			printf(" » ");
		}
		
		char* input = getInput();
		if(strlen(input) == 0){
			free(input);
			continue;
		}
		list_insert(h, c++, input);

		List arglist = tokenize(input);
		char** args = list_array(arglist);
		char* cmd = args[0];
		if ((strcasecmp(cmd, "exit")==0) || (strcasecmp(cmd, "logout")==0)) {
			canRun = 0;
		} else if (strcasecmp(cmd, "history")==0) {
			LItem* li;
			for (li = list_getFirst(h); li != NULL; li = li->next) {
				printf("%2d %s\n", li->index, li->value);
			}
		} else {
			printf("Received command: %s\n", cmd);
		}

		free(input);
		list_destroy(arglist);
		free(args);
	}
	list_destroy(h);
	return 0;
}

List tokenize(char* input) {
	List args = list_create(0);
	char* token = strtok(input, " ");
	while (token != NULL) {
		list_insert(args, 0, token);
		token = strtok(NULL, " ");
	}
	list_insert(args, 0, NULL);
	return args;
}

char* getInput() {
	char input[BUFSIZ], *p;
	if(fgets(input, sizeof(input), stdin) != NULL){
		if((p = strchr(input, '\n')) != NULL){
			*p = '\0';
		}
	}
	char* s = strdup(input);
	return s;
}
