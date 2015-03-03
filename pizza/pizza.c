/**
 * Douglas Skrypa
 * Version: 2015.03.03
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
#define DELIVERY_MIN 2	//Minimum number of seconds required for pizza delivery
#define DELIVERY_MAX 5	//Maximum number of seconds required for pizza delivery

int studentCount, sliceCount;
int remainingSlices, activeStudents;
enum {PIZZA,ORDER,STUDENTS};	//Enum for lock semaphore clarity
sem_t locks[3];					//Lock semaphores for synchronized variables

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
	
	int i;	//Initialize semaphores for locks
	for (i = 0; i < 3; i++) {
		sem_init(&(locks[i]), 0, 1);
	}

	srand(time(NULL));      	//Seeds random number generator

	//Declare & initialize thread attributes
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	//Create threads
	if (pthread_create(&parlor_thread, &attr, pizzaParlor, NULL) != 0) {
		errexit("Unable to create pizza parlor thread.\n");
	}
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
	for (i = 0; i < 3; i++) {
		sem_destroy(&(locks[i]));
	}
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
		int rem = get_slice();		//Get a slice, if possible
		if (rem == -1) {			//If there were no available slices
			sleep(1);					//Wait before checking aagin
		} else {					//Otherwise, we got a slice
			printf("S%d: Got a slice; there are %d left.\n", (int)tid, rem);
			slices++;					//Increment the number eaten
			if (rem == 0) {				//If we got the last slice
				placeOrder();				//Place order
				printf("S%d: I ordered another pizza.\n", (int)tid);
			}
			sleep(rand()%EAT_MAX+1);	//Eat the slice of pizza
		}
	}
	//Ate as many slices as necessary
	printf("S%ld: I'm done studying.\n", tid);
	sem_wait(&(locks[STUDENTS]));	//Wait for lock on students
		activeStudents--;				//Decrement the number of active students
	sem_post(&(locks[STUDENTS]));	//Release lock on students
	pthread_exit(NULL);				//Terminate the thread
}

/**
	Thread-safe function to place an order for a new pizza
*/
void placeOrder() {
	sem_wait(&(locks[ORDER]));	//Wait for lock on order status
		orderPlaced = true;
	sem_post(&(locks[ORDER]));	//Release lock on order status
}

/**
	Thread-safe check for order status
	@return true if an order has ben placed, false otherwise
*/
bool hasOrder() {
	bool ordered;
	sem_wait(&(locks[ORDER]));	//Wait for lock on order status
		ordered = orderPlaced;
	sem_post(&(locks[ORDER]));	//Release lock on order status
	return ordered;
}

/**
	Thread-safe check for the number of students that are active
	@return true if activeStudents > 0, false otherwise
*/
bool stillActive() {
	bool active;
	sem_wait(&(locks[STUDENTS]));	//Wait for lock on students
		active = activeStudents > 0;
	sem_post(&(locks[STUDENTS]));	//Release lock on students
	return active;
}

/**
	Pizza Parlor thread
*/
void* pizzaParlor() {
	while (stillActive()) {	//Loop while there are students studying
		if (hasOrder()) {			//Catch an incoming order
			make_pizza();			//Fulfill the order
		}
	}
	pthread_exit(NULL);				//Terminate the thread
}

/**
	Thread-safe function to simulate cooking / delivering a pizza
	Spend a random (bounded) amount of time sleeping to simulate delivery time,
	then refresh the number of slices that are available.
*/
void make_pizza() {
	sem_wait(&(locks[ORDER]));	//Wait for lock on order status
		orderPlaced = false;
	sem_post(&(locks[ORDER]));	//Release lock on order status
	
	sleep(rand()%DELIVERY_MAX+DELIVERY_MIN);
	sem_wait(&(locks[PIZZA]));	//Wait for lock on pizza
		printf("Pizza Parlor: Delivered a fresh pizza with %d slices.\n", sliceCount);
		remainingSlices = sliceCount;
	sem_post(&(locks[PIZZA]));	//Release lock on pizza
}

/**
	Thread-safe function to get a slice of pizza
	@return the number of slices remaining or -1 if there were no slices available
*/
int get_slice() {
	int retval = -1;
	sem_wait(&(locks[PIZZA]));	//Wait for lock on pizza
		if (remainingSlices > 0) {
			remainingSlices--;
			retval = remainingSlices;
		}
	sem_post(&(locks[PIZZA]));	//Release lock on pizza
	return retval;
}

/**
	Shortcut function for exiting after printing an error message.
*/
void errexit(char* msg) {
	perror(msg);
	exit(1);
}
