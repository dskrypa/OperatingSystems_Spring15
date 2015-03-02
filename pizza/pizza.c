#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MAXSLICES 12    //Maximum number of slices that a student will eat
#define EAT_MAX 4       //Maximum number of seconds required to eat
#define DELIVERY_MIN 3  //Minimum number of seconds required for pizza delivery
#define DELIVERY_MAX 10 //Maximum number of seconds required for pizza delivery

int studentCount = 4;    //N
int sliceCount = 8;      //S
int remainingSlices = sliceCount;
int activeStudents = studentCount;

pthread_t student_threads[studentCount];
pthread_t parlor_thread;

enum {IN_LINE, PICKING, EATING} state[studentCount];
pthread_cond_t student_cv[studentCount];
pthread_mutex_t mutex;

enum {AVAILABLE, EMPTY, ORDERED, IN_TRANSIT, DONE} pizza;
pthread_cond_t parlor_cv;

int main() {
	//Initialize mutex
	if (pthread_mutex_init(&mutex, NULL) != 0) {
		errexit("Unable to create mutex.\n");
	}

	//Initialize pizza parlor vars
	pizza = AVAILABLE;
	if (pthread_cond_init(&parlor_cv, NULL) != 0) {
		errexit("Unable to create pizza parlor condition variable.\n");
	}

	//Initialize student vars
	int i;
	for (i = 0; i < studentCount; i++) {
		state[i] = IN_LINE;
		if (pthread_cond_init(&(student_cv[i]), NULL) != 0) {
			errexit("Unable to create student condition variable.\n");
		}
	}

	srand(time());      //Seeds random number generator

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
	if (pthread_join(&parlor_thread, NULL) != 0) {
		errexit("Unable to join pizza parlor thread.\n");
		exit(1);
	}
	for (i = 0; i < studentCount; i++) {
		if (pthread_join(student_threads[i], NULL) != 0) {
			errexit("Unable to join student thread.\n");
		}
	}

	//Cleanup
	printf("The students are done studying & the pizza parlor is closed.\n");
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&parlor_cv);
	for (i = 0; i < studentCount; i++) {
		pthread_cond_destroy(&(student_cv[i]))
	}
	pthread_exit(NULL);
	return 0;
}

void* student(void* threadid) {
	long tid = (long) threadid;
	//Iterate for a random number of slices of pizza
	int i;
	for (i = 0 i < rand()%MAXSLICES+1; i++) {
        take_slice((int)tid);		//Get in line to take a slice of pizza
        sleep(rand()%EAT_MAX+1);	//Eat the slice of pizza
	   //i'm hungry
	   //wait in line for pizza
	   //get pizza
	   //if i took the last slice, order more
	   //eat pizza

	}
	//Done studying
	printf("S%d: Done studying.\n", tid);
	activeStudents--;
	if (activeStudents == 0) {
		pizza = DONE;
	}
	pthread_exit(NULL);
}

void* pizzaParlor() {
	while (pizza != DONE) {
		if (pizza = ORDERED) {
			make_pizza();
		}
	}
	pthread_exit(NULL);
}

void make_pizza() {
	sleep(rand()%DELIVERY_MAX+DELIVERY_MIN);
	printf("Pizza Parlor: Delivered a fresh pizza with %d slices.\n", sliceCount);
	remainingSlices = sliceCount;
	pizza = AVAILABLE;
}

/**
	Synchronized function for taking a slice of pizza
	@param i the ID number of the student attempting to take a slice of pizza
*/
void take_slice(int i) {
	pthread_mutex_lock(&mutex);
	state[i] = IN_LINE;					//Student i is in line for a slice of pizza
	printf("S%d: I'm hungry.\n", i);	//Announce status
	//Attempt to obtain lock on pizza
	test(i);
	while (state[i] != PICKING) {
		pthread_cond_wait(&(student_cv[i]), &mutex);
	}
	//Obtained lock on pizza
	remainingSlices -= 1;				//Decrement number of slices remaining
	printf("S%d: I got a slice; there are %d slices left.\n", i, remainingSlices);	//Announce status
	//Update pizza's status
	if (remainingSlices == 0) {
		pizza = ORDERED;
		printf("S%d: I ordered another pizza.\n", i);
	}
	//Release lock on pizza
	pthread_mutex_unlock(&mutex);
}

void test(int i) {
	if ((state[(i-1+studentCount)%studentCount] != PICKING)
		&& (state[i] == IN_LINE)
		&& (state[(i+1)%studentCount] != PICKING)
		&& (pizza == AVAILABLE)
	){
		state[i] = PICKING;
		pthread_cond_signal(&(student_cv[i]));
	}
}

void errexit(char* msg) {
	perror(msg);
	exit(1);
}
