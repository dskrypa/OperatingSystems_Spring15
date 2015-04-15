Operating Systems, Homework 4
Douglas Skrypa
2015.04.14

Files included:
LinkedList.c  makefile     paging.c  readme.txt     stringUtils.h
LinkedList.h  modernize.h  paging.h  stringUtils.c

================================================================================

Compile:

make

Run:

./paging

================================================================================

I used my old LinkedList implementation for ease of converting the user input
for the page reference stream into an array.  I tested my program with Valgrind
for all possible exit points to make sure that there were no memory leaks.

I implemented each replacement algorithm in its own function, with each one
returning the number of faults that they incurred.  They each print the status
of the page frames after each page request.

In the process of debugging, I discovered that in the loops that iterate through
the frame arrays for the purpose of determining whether or not there was a page
hit, I needed to make sure that the frame array index was less than the current
page reference stream index, or the default value of 0 given to that element of
the frame array could cause errors where it would match the value of the page
that was being requested.

Everything now works as intended, with any input / page frame count.
