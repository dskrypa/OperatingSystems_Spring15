/**
 * Douglas Skrypa
 * Version: 2015.04.14
 * Homework 4: Page Replacement Algorithms Simulation
 */

#include <stdio.h>					//IO functions
#include <stdlib.h>					//Memory functions
#include <string.h>					//String functions
#include <errno.h>					//Access to stderr stream

#include "modernize.h"				//Defines bool & String
#include "stringUtils.h"			//String comparison & manipulation
#include "LinkedList.h"				//Linked List implementation

#include "paging.h"



int main(int argc, String* argv) {
	printf("Enter page reference stream:\n");
	String refStream_raw = getInput();
	
	//Tokenize page reference stream input
	List refStream_list = tokenize(refStream_raw);
	String* refStream_arr = list_array(refStream_list);
	int pagec = list_size(refStream_list);
	if (pagec < 1) {
		fprintf(stderr, "Too few arguments provided.\n");
		exit(1);
	}
	int refStream[pagec];
	
	//Convert page reference stream array to integers
	int p;
	for (p = 0; p < pagec; p++) {
		int v;
		if (sscanf(refStream_arr[p], "%d", &v) == 1) {
			refStream[p] = v;
		} else {
			fprintf(stderr, "Invalid page refrence value: %s (Must be an integer)\n", refStream_arr[p]);
			exit(1);
		}
	}
	
	//Get and validate page frame count input
	printf("Enter number of page frames: ");
	String framec_raw = getInput();
	int framec;
	if (sscanf(framec_raw, "%d", &framec) != 1) {
		fprintf(stderr, "Invalid page frame count: %s (Must be an integer)\n", framec_raw);
		exit(1);
	}
	
	//Get and validate page replacement algorithm
	printf("Enter page replacement algorithm (FIFO, LRU, OPT): ");
	String repAlg_raw = getInput();
	enum {FIFO,LRU,OPT} algorithm;
	if (streqic("FIFO", repAlg_raw)) {
		algorithm = FIFO;
	} else if (streqic("LRU", repAlg_raw)) {
		algorithm = LRU;
	} else if (streqic("OPT", repAlg_raw)) {
		algorithm = OPT;
	} else {
		fprintf(stderr, "Invalid page replacement algorithm: %s\n", repAlg_raw);
		exit(1);
	}
	printf("Algorithm: %d", algorithm);
	
	printf("\nInput:");
	for (p = 0; p < pagec; p++) {
		printf(" %d", refStream[p]);
	}
	printf("\n");
	return 0;
}


List tokenize(String input) {
	List args = list_create(0);
	String token = strtok(input, " ");
	while (token != NULL) {
		list_insert(args, 0, token);
		token = strtok(NULL, " ");
	}
	return args;
}


String getInput() {
	char input[BUFSIZ], *p;
	if(fgets(input, sizeof(input), stdin) != NULL){
		if((p = strchr(input, '\n')) != NULL){
			*p = '\0';
		}
	}
	String s = strdup(input);
	return s;
}
