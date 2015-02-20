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
#include <signal.h>
#include <sys/wait.h>

#include "LinkedList.h"
#include "stringUtils.h"
#include "shell.h"

List h;
unsigned c;

int main(int argc, string* argv){
	struct sigaction act;			//Define a struct to handle signals
	act.sa_handler = SIG_IGN;		//Set the action to ignore
	act.sa_flags = 0;				//Set no options
	sigemptyset(&act.sa_mask);		//Initialize the sigaction mask
	sigaction(SIGINT, &act, NULL);	//Ignore [ctrl]+[c]
	sigaction(SIGTSTP, &act, NULL);	//Ignore [ctrl]+[z]

	printf("DS Shell\n");
	unsigned canRun = 1;
	h = list_create(10);
	c = 0;
	while(canRun) {
		char wd[2048];
		if (getcwd(wd, sizeof(wd)) != NULL) {
			printf("%s » ", wd);
		} else {
			printf(" » ");
		}
		
		string input = getInput();
		if((strlen(input) == 0) || (input == NULL)){
			free(input);
			continue;
		}
		
		if (streqic(input, "exit") || streqic(input, "logout")) {
			canRun = 0;
			free(input);
		} else {
			processInput(input);
		}
		
	}
	list_destroy(h);
	return 0;
}

void processInput(string input) {
	if (input[0]!='!') {
		list_insert(h, c++, input);
	}
	
	List arglist = tokenize(input);
	string* args = list_array(arglist);
	
	if (streqic(args[0], "history")) {
		LItem* li;
		for (li = list_getFirst(h); li != NULL; li = li->next) {
			printf("%2d %s\n", li->index, li->value);
		}
	} else if (args[0][0] == '!') {
		string hinput = NULL;
		if (streq(args[0], "!!")) {
			hinput = list_getLastVal(h);
			if (hinput == NULL) {
				printf("Command not found\n");
			}
		} else if (strlen(args[0]) > 1) {
			string num = substring(args[0],1,0);
			int n = atoi(num);
			free(num);
			hinput = list_getVal(h, n);
			if (hinput == NULL) {
				printf("%d: Command not found\n", n);
			}
		}
		if (hinput != NULL) {
			processInput(hinput);
		}
	} else {
		if (list_contains(arglist, "|")) {
			printf("Contains pipe!\n");
		}
	
		runCommand(args);
	}
	
	list_destroy(arglist);
	free(args);
	free(input);
}

void runCommand(string* args) {
	pid_t pid;
	int status;
	switch (pid = fork()) {
		case 0:
			status = execvp(*args, args);
			break;
		default:
			while(wait(&status) != pid){}
			break;
		case -1:
			perror("ERROR: Unable to execute command.\n");
			break;
	}
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
