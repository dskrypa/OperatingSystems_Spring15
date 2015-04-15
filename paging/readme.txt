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

Each algorithm was given its own function that returns the number of faults that
were encountered.  They all start by initializing an array of page frames to
store the pages that are loaded in memory, then iterating through the list of
pages being requested in the page reference stream.  What they do while
processing that list is where they differ.  At the end of each iteration, they
print the page that was requested, and the contents of the frame array.

FIFO:
Iterate through filled frames to find a hit; if a hit isn't found, increment the
last replacement index variable, and set the frame with that index to the
current page value.

LRU:
Iterate through filled frames to find a hit; if a hit is found, set the distance
at that index to 0, otherwise increment the distance at that index by 1.  If a 
fault is found, and the number of faults is less than the frame array size, set
the frame value at index faults to the page value, and the distance at that
index to 0, otherwise find the index of the element in frames with the largest
backward distance, and do the same for that index.

OPT:
Iterate through the reference stream from the current point to the end to
determine how far away the next page with the same value is, with a default
distance of INT_MAX.  Iterate through filled frames to find a hit; if a hit is
found, set the distance at that index to the distance that was just determined,
otherwise decrement the distance at that index by 1 if it's not INT_MAX.  If a
fault is found, and the number of faults is less than the frame array size, set
the frame value at index faults to the page value, and the distance at that
index to the the distance that was just determined, otherwise find the index of
the element in frames with the largest forward distance, and do the same for
that index.

In the process of debugging, I discovered that in the loops that iterate through
the frame arrays for the purpose of determining whether or not there was a page
hit, I needed to make sure that the frame array index was less than the number
of page faults encountered, which is the same as the number of frame slots that
have been filled, otherwise the default value of 0 given to that element of the
frame array by the compiler could cause errors where it would match the value of
the page that was being requested.

Everything now works as intended, with any input / page frame count.
