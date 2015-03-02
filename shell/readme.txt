Operating Systems, Homework 1
Douglas Skrypa
2015.02.19

Files included:
LinkedList.c	LinkedList.h	makefile		readme.txt
shell.c			shell.h			stringUtils.c	stringUtils.h

================================================================================

Compile:

make

Run:

./shell

================================================================================

I started by writing a LinkedList implementation to handle splitting user inputs
of variable length into Lists.  I also use it to store my command history.
Setting a maximum size for the List and having the default action to over-write
in a first-in-first-out fashion made it easy to maintain the history size limit.

I expanded upon some utilities for working with strings that I had written
previously to include functions for comparing two strings, with a return type of
_Bool so that I would not have to check that strcmp returned 0 each time.

I was able to implement all of the desired functionality for the shell, except
for using redirects, due to poor planning of time use on my part.  I have tested
it in Valgrind, and there are no memory leaks or other issues unless a given
fork is terminated due to bad input, but this leakage is local to the fork in 
which it happened.
