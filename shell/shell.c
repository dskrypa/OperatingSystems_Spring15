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
#include "stringUtils.h"
#include "shell.h"

int main(int argc, string* argv){
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
		
		string input = getInput();
		if(strlen(input) == 0){
			free(input);
			continue;
		}
		
		List arglist = tokenize(input);
		string* args = list_array(arglist);
		string cmd = args[0];
		if (streqic(cmd, "exit") || streqic(cmd, "logout")) {
			canRun = 0;
		} else if (streqic(cmd, "history")) {
			LItem* li;
			for (li = list_getFirst(h); li != NULL; li = li->next) {
				printf("%2d %s\n", li->index, li->value);
			}
		} else if (streq(substring(cmd,0,1),"!")) {		//This will leak
			if (streq(cmd,"!!")) {
				printf("%s\n", list_getLast(h)->value);
			} else {
				printf("xx\n");
			}
		} else {
			printf("Received command: %s\n", cmd);
		}
		list_insert(h, c++, input);
		
		free(input);
		list_destroy(arglist);
		free(args);
	}
	list_destroy(h);
	return 0;
}

List tokenize(string input) {
	List args = list_create(0);
	string token = strtok(input, " ");
	while (token != NULL) {
		list_insert(args, 0, token);
		token = strtok(NULL, " ");
	}
	list_insert(args, 0, NULL);
	return args;
}

string getInput() {
	char input[BUFSIZ], *p;
	if(fgets(input, sizeof(input), stdin) != NULL){
		if((p = strchr(input, '\n')) != NULL){
			*p = '\0';
		}
	}
	string s = strdup(input);
	return s;
}
