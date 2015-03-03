/**
 * Douglas Skrypa
 * Version: 2015.03.02
 * Homework 2: Pizza Parlor
 */

#include "modernize.h"	//Defines bool & String

#include <pthread.h>	//Multithreading
#include <semaphore.h>	//Semaphores for synchronization
#include <stdio.h>		//IO functions
#include <stdlib.h>		//Memory functions
#include <unistd.h>		//Misc types
#include <time.h>		//Sleep functions

#include "pizza.h"		//This program's header file

#define MAXSLICES 6		//Maximum number of slices that a student will eat
#define EAT_MAX 4		//Maximum number of seconds required to eat
#define DELIVERY_MIN 3	//Minimum number of seconds required for pizza delivery
#define DELIVERY_MAX 10	//Maximum number of seconds required for pizza delivery

int studentCount, sliceCount;
int remainingSlices, activeStudents;
sem_t slice, order;
bool orderPlaced;

int main(int argc, String* argv) {
	//Input validation
	if (argc < 3) {
		errexit("Too few arguments provided.\n");
	}
	sscanf(argv[1], "%d", &studentCount);
	if (studentCount < 1) {
		errexit("Invalid argument for number of students.\n");
	}
	sscanf(argv[2], "%d", &sliceCount);
	if (sliceCount < 1) {
		errexit("Invalid argument for number of slices.\n");
	}

	printf("There are %d students studying.\n", studentCount);
	printf("There is a pizza with %d slices available.\n", sliceCount);

	//Init variables
	orderPlaced = false;
	remainingSlices = sliceCount;
	activeStudents = studentCount;
	
	pthread_t student_threads[studentCount];
	pthread_t parlor_thread;
	
	sem_init(&slice, 0, 1);		//Initialize semaphore for slice picking
	sem_init(&order, 0, 1);		//Initialize semaphore for pizza ordering

	srand(time(NULL));      	//Seeds random number generator

	//Declare & initialize thread attributes
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	//Create threads
	if (pthread_create(&parlor_thread, &attr, pizzaParlor, NULL) != 0) {
		errexit("Unable to create pizza parlor thread.\n");
	}
	int i;
	for (i = 0; i < studentCount; i++) {
		long t = (long) i;
		if (pthread_create(&(student_threads[i]), &attr, student, (void*)t) != 0) {
			errexit("Unable to create student thread.\n");
		}
	}

	//Join threads
	if (pthread_join(parlor_thread, NULL) != 0) {
		errexit("Unable to join pizza parlor thread.\n");
	}
	for (i = 0; i < studentCount; i++) {
		if (pthread_join(student_threads[i], NULL) != 0) {
			errexit("Unable to join student thread.\n");
		}
	}

	//Cleanup
	printf("The students are done studying & the pizza parlor is closed.\n");
	pthread_attr_destroy(&attr);
	sem_destroy(&slice);
	sem_destroy(&order);
	pthread_exit(NULL);
	return 0;
}

/**
	Student thread
	@param threadid the ID for this student thread
*/
void* student(void* threadid) {
	long tid = (long) threadid;
	int slices = 0;
	int lastSlices = -1;
	int maxSlices = rand()%MAXSLICES+1;
	//Loop while student hasn't eaten their fill of pizza
	while (slices < maxSlices) {
		if (lastSlices != slices) {	//Print hunger message once per slice
			lastSlices = slices;
			printf("S%d: I'm hungry!\n", (int)tid);
		}
		
		sem_wait(&slice);			//Wait for lock on pizza
			int rem = get_slice();		//Get a slice, if possible
		sem_post(&slice);			//Release lock on pizza
		
		if (rem == -1) {			//If there were no available slices
			sleep(1);					//Wait before checking aagin
		} else {					//Otherwise, we got a slice
			printf("S%d: Got a slice; there are %d left.\n", (int)tid, rem);
			slices++;					//Increment the number eaten
			if (rem == 0) {				//If we got the last slice
				sem_wait(&order);			//Wait for lock on order
					placeOrder();				//Place order
					printf("S%d: I ordered another pizza.\n", (int)tid);
				sem_post(&order);			//Release lock on order
			}
			sleep(rand()%EAT_MAX+1);	//Eat the slice of pizza
		}
	}
	//Ate as many slices as necessary
	printf("S%ld: Done studying.\n", tid);
	activeStudents--;				//Decrement the number of active students
	pthread_exit(NULL);				//Terminate the thread
}

/**
	Place an order with the pizzaParlor for a new pizza
*/
void placeOrder() {
	orderPlaced = true;
}

/**
	Pizza Parlor thread
*/
void* pizzaParlor() {
	while (activeStudents > 0) {	//Loop while there are students studying
		if (orderPlaced) {			//Catch an incoming order
			orderPlaced = false;
			make_pizza();			//Fulfill the order
		}
	}
	pthread_exit(NULL);				//Terminate the thread
}

/**
	Make a pizza
	Spend a random (bounded) amount of time sleeping to simulate delivery time,
	then refresh the number of slices that are available.
*/
void make_pizza() {
	sleep(rand()%DELIVERY_MAX+DELIVERY_MIN);
	sem_wait(&slice);			//Wait for lock on pizza
		printf("Pizza Parlor: Delivered a fresh pizza with %d slices.\n", sliceCount);
		remainingSlices = sliceCount;
	sem_post(&slice);			//Release lock on pizza
}

/**
	Get a slice of pizza
	@return the number of slices remaining or -1 if there were no slices available
*/
int get_slice() {
	if (remainingSlices > 0) {
		remainingSlices--;
		return remainingSlices;
	}
	return -1;
}

/**
	Shortcut function for exiting after printing an error message.
*/
void errexit(char* msg) {
	perror(msg);
	exit(1);
}
