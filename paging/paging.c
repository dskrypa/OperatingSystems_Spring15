/**
 * Douglas Skrypa
 * Version: 2015.04.14
 * Homework 4: Page Replacement Algorithms Simulation
 */

#include <stdio.h>					//IO functions
#include <stdlib.h>					//Memory functions
#include <string.h>					//String functions
#include <errno.h>					//Access to stderr stream
#include <limits.h>					//Limits for types

#include "modernize.h"				//Defines bool & String
#include "stringUtils.h"			//String comparison & manipulation
#include "LinkedList.h"				//Linked List implementation

#include "paging.h"

int main(int argc, String* argv) {
	printf("Enter page reference stream:\n");
	String refStream_raw = getInput();
	
	//Tokenize page reference stream input
	List refStream_list = tokenize(refStream_raw);
	free(refStream_raw);
	String* refStream_arr = list_array(refStream_list);
	int pagec = list_size(refStream_list);
	if (pagec < 1) {
		fprintf(stderr, "Too few arguments provided.\n");
		list_destroy(refStream_list);
		free(refStream_arr);
		exit(1);
	}
	int* refStream = malloc(pagec * sizeof(int));
	
	//Convert page reference stream array to integers
	int p;
	for (p = 0; p < pagec; p++) {
		int v;
		if (sscanf(refStream_arr[p], "%d", &v) == 1) {
			refStream[p] = v;
		} else {
			fprintf(stderr, "Invalid page refrence value: %s (Must be an integer)\n", refStream_arr[p]);
			list_destroy(refStream_list);
			free(refStream_arr);
			free(refStream);
			exit(1);
		}
	}
	list_destroy(refStream_list);
	free(refStream_arr);
	
	//Get and validate page frame count input
	printf("Enter number of page frames: ");
	String framec_raw = getInput();
	int framec;
	if ((sscanf(framec_raw, "%d", &framec) != 1) || (framec < 1) || (framec > 10)) {
		fprintf(stderr, "Invalid page frame count: %s (Must be an integer between 1 and 10)\n", framec_raw);
		free(framec_raw);
		free(refStream);
		exit(1);
	}
	free(framec_raw);
	
	//Get and validate page replacement algorithm
	printf("Enter page replacement algorithm (FIFO, LRU, OPT): ");
	String repAlg_raw = getInput();
	int faults;
	if (streqic("FIFO", repAlg_raw)) {
		faults = use_fifo(refStream, pagec, framec);
	} else if (streqic("LRU", repAlg_raw)) {
		faults = use_lru(refStream, pagec, framec);
	} else if (streqic("OPT", repAlg_raw)) {
		faults = use_opt(refStream, pagec, framec);
	} else {
		fprintf(stderr, "Invalid page replacement algorithm: %s\n", repAlg_raw);
		free(repAlg_raw);
		free(refStream);
		exit(1);
	}
	
	printf("\nNumber of page faults: %d\n", faults);
	printf("Number of page references: %d\n", pagec);
	float ratio = (float)faults / (float)pagec;
	printf("Fault ratio: %.3f\n", ratio);
	
	//Release memory & return
	free(repAlg_raw);
	free(refStream);
	return 0;
}

/**
	Simulates the FIFO page replacement algorithm
	@param refStream an array of page numbers
	@param refc the number of elements in refStream
	@param framec the number of frames to be used
*/
int use_fifo(int* refStream, int refc, int framec) {
	printf("FIFO Page Replacement\nPage\tFrames");
	int* frame = malloc(framec * sizeof(int));	//Malloc to pass to print func
	int faults = 0;
	int lastRep = framec-1;

	int c;	//Iterate through the given pages
	for (c = 0; c < refc; c++) {
		bool hit = false;
		int f;	//Determine if there is a hit or fault
		for (f = 0; (f < faults) && (f < framec); f++) {
			if (frame[f] == refStream[c]) {
				hit = true;
				break;
			}
		}
		
		if (!hit) {	//Determine frame to replace
			int rep = (lastRep + 1 < framec) ? lastRep + 1 : 0;
			frame[rep] = refStream[c];
			lastRep = rep;
			faults++;
		}
		printState(refStream[c], frame, framec, !hit, faults);
	}
	
	free(frame);
	return faults;
}

/**
	Simulates the LRU page replacement algorithm
	@param refStream an array of page numbers
	@param refc the number of elements in refStream
	@param framec the number of frames to be used
*/
int use_lru(int* refStream, int refc, int framec) {
	printf("LRU Page Replacement\nPage\tFrames");
	int* frame = malloc(framec * sizeof(int));	//Malloc to pass to print func
	int faults = 0;
	
	int prev[framec];
	
	int c;	//Iterate through the given pages
	for (c = 0; c < refc; c++) {
		bool hit = false;
		int f;	//Determine if there is a hit or fault & increment distances
		for (f = 0; (f < faults) && (f < framec); f++) {
			if (frame[f] == refStream[c]) {
				hit = true;
				prev[f] = 0;
			} else {
				prev[f]++;
			}
		}
		
		if (!hit) {	//Determine frame to replace
			if (faults < framec) {
				frame[faults] = refStream[c];
				prev[faults] = 0;
			} else {
				int furthest = 0;
				for (f = 0; f < framec; f++) {
					if (prev[f] > prev[furthest]) {
						furthest = f;
					}
				}
				frame[furthest] = refStream[c];
				prev[furthest] = 0;
			}
			faults++;
		}
		printState(refStream[c], frame, framec, !hit, faults);
	}
	
	free(frame);
	return faults;
}

/**
	Simulates the OPT page replacement algorithm
	@param refStream an array of page numbers
	@param refc the number of elements in refStream
	@param framec the number of frames to be used
*/
int use_opt(int* refStream, int refc, int framec) {
	printf("OPT Page Replacement\nPage\tFrames");
	int* frame = malloc(framec * sizeof(int));	//Malloc to pass to print func
	int faults = 0;
	int next[framec];
	
	int c;	//Iterate through the given pages
	for (c = 0; c < refc; c++) {
		int nextp = INT_MAX;
		int p;
		for (p = c + 1; p < refc; p++) {
			if (refStream[p] == refStream[c]) {
				nextp = p - c;
				break;
			}
		}
	
		bool hit = false;
		int f;	//Determine if there is a hit or fault & decrement distances
		for (f = 0; (f < faults) && (f < framec); f++) {
			if (frame[f] == refStream[c]) {
				hit = true;
				next[f] = nextp;
			} else {
				next[f] = (next[f] < INT_MAX) ? next[f] - 1 : INT_MAX;
			}
		}
		
		if (!hit) {	//Determine frame to replace
			if (faults < framec) {
				frame[faults] = refStream[c];
				next[faults] = nextp;
			} else {
				int furthest = 0;
				for (f = 1; f < framec; f++) {
					if (next[f] > next[furthest]) {
						furthest = f;
					}
				}
				frame[furthest] = refStream[c];
				next[furthest] = nextp;
			}
			faults++;
		}
		printState(refStream[c], frame, framec, !hit, faults);
	}
	
	free(frame);
	return faults;
}

/**
	Handles getting user input
	@return a String containing the user's input
*/
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

/**
	Tokenizes a String of user input into a List
	@param input user-provided input
	@return a List of the elements of input that were separated by spaces
*/
List tokenize(String input) {
	List args = list_create(0);
	String token = strtok(input, " ");
	while (token != NULL) {
		list_insert(args, 0, token);
		token = strtok(NULL, " ");
	}
	return args;
}

/**
	Prints the current state of the page replacement process
	@param page the current page request
	@param frame the array of frames
	@param framec the total number of frames
	@param fault true if a page fault was encountered
	@param framesFilled the number of frames that have been initialized
*/
void printState(int page, int* frame, int framec, bool fault, int framesFilled) {
	printf("\n%d\t", page);
	int f;
	for (f = 0; f < framec; f++) {
		if (f < framesFilled) {
			printf("%d ", frame[f]);
		} else {
			printf("- ");
		}
	}
	if (fault) {
		printf("\tFault");
	}
}
