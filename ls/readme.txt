Operating Systems, Homework 6
Douglas Skrypa
2015.05.04

Files included:
makefile  modernize.h  myls.c  myls.h  stringUtils.c  stringUtils.h

================================================================================

Compile:

make

Run:

./myls [-i | -l | -R]

================================================================================

My program runs as expected and there are no memory leaks reported by Valgrind.
Each version of ls was implemented in its own function.  I ended up adding a
sorting function to my stringUtils, but ended up making it use stdlib's qsort
once I figured out how to use it.  I also added a String concatenation function
that accepts a variable number of Strings.  This made reconstructing file paths
easier.

Honestly, I could clean up some of the code for the implementations of -l and
-R to get rid of some redundancy / silly ordering of doing things...

All 4 implementations begin with populating an array with the names of the items
in the given folder, and then sorting that list alphabetically.  Due to the way
they are implemented, they could easily accept paths for an arbitrary location
in the future.  The only time the current working directory is referenced is in
main.  

ls
Simply prints the sorted list.

ls -i
Gets the inode number, and prints it along with its name.  A new line character
is added periodically.

ls -l
Gathers information about the file and converts it into human-readable output.
Some extra libraries needed to be included for these conversions.

ls -R
I implemented this one recursively, informing it of whether or not it is the
first call, then passing along the header that should be used for each directory
before printing its contents.  I discovered the problem with my initial sorting
algorithm while testing this one, and then discovered how to make qsort work.
