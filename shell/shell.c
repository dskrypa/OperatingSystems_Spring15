/**
 * Douglas Skrypa
 * Assignment 1: Shell
 * 2015.02.19
 */
 
#include "LinkedList.h"			//Misc commonly needed utilities that I wrote

typedef char* string;

int main(int argc, string* argv) {
	printf("DS Shell\n");
	unsigned canRun = 1, c = 0;
	List h = list_create(10);
	while(canRun) {
		string wd;
		if (getcwd(wd, 0) == null) {
			wd = ''
		}
		printf("%s » ", wd);
		
		string input = getInput();
		if(strlen(input) == 0){
			free(input);
			continue;
		}
		list_insert(h, c++, input);
		
		//List arglist = tokenize(input);
		//string* args = list_array(arglist);
		string* args = tokenize(input);
		string cmd = args[0];
		if ((strcasecmp(cmd, "exit")==0) || (strcasecmp(cmd, "logout")==0)) {
			canRun = 0;
		} else if (strcasecmp(cmd, "history")==0) {
			LItem li;
			for (li = h->first; li != NULL; li = li->next) {
				printf("%2d %s\n", li->index, li->value);
			}
		} else {
			printf("Received input: %s\n", input);
		}

		free(input);
		//list_destroy(arglist);
		free(args);
	}
	list_destroy(h);
	return 0;
}

//List tokenize(string input) {
string* tokenize(string input) {
	List args = list_create(0);
	string token = strtok(input, " ");
	while (token != NULL) {
		list_insert(args, 0, token);
		token = strtok(NULL, " ");
	}
	list_insert(args, 0, NULL);
	string* argarray = list_array(args);
	list_destroy(args);
	return argarray;
	//return args;
}

/**
	Retrieve a string of user input from stdin
	@return a string of user input
*/
string getInput() {
	char input[BUFSIZ], *p;				//Use built-in BUFSIZ for buffer size
	if(fgets(input, sizeof(input), stdin) != NULL){	//Get input
		if((p = strchr(input, '\n')) != NULL){
			*p = '\0';			//Remove trailing \n if necessary
		}
	}
	string s = strdup(input);
	return s;
}
