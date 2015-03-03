Operating Systems, Homework 2
Douglas Skrypa
2015.03.03

Files included:
makefile		modernize.h		pizza.c		pizza.h
readme.txt

================================================================================

Compile:

make

Run:

./pizza <# students> <# slices>

================================================================================

Pseudocode Solution:

student(int i) {
	while (hungry) {
		wait_for_then_get_slice()
		if (took_last_slice) {
			order_more_pizza()
		}
		eat()
	}
}

pizzaParlor() {
	while (students_are_studying()) {
		if (got_order()) {
			make_and_deliver_pizza()
		}
	}
}

================================================================================

Everything works as intended.  I used semaphores for synchronization of the
shared pizza, order status, and number of active students.  I discovered that
valgrind does not handle multi-threaded applications well due to forcing
everything to run on a single thread.

I initially tried to solve this problem with the same method as the Dining
Philosophers problem, but that method of notification did not seem to work well
for this situation.  A specific student's ability to get a slice of pizza is not
tied to their position in the group of other students, as was the case with the
philosophers.  Additionally, only one student is able to get a slice of pizza at
a time, which would make having condition variables for each student a waste of
space.  Instead, I used semaphores to regulate access to and modification of key
variables, such as the number of slices of pizza remaining, and the number of
students who are still active.

