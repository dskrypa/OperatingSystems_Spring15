/**
 * Douglas Skrypa
 * Version: 2015.05.04
 * Homework 6: Implementing ls
 
 DUE: Tuesday May 5th @ 11:55 PM
 
 */

#include <stdio.h>					//IO functions
#include <stdlib.h>					//Memory functions; exit()
#include <unistd.h>					//Misc symbolic constants, types, and functions
#include <errno.h>					//Access to stderr stream
#include <dirent.h>					//Directory access

#include "modernize.h"				//Defines bool & String
#include "stringUtils.h"			//String comparison & manipulation

#include "myls.h"

/*
ls			list the names of files and directories in the current directory
ls -i		list the names and i-node numbers of files and directories in the current directory
ls -l		list the names of files and directories in long format
ls -R		recursively list the names of all files and directories in the current directory; that is, the command is also applied to the directories within the current directory, the directories within those directories, and so forth

System calls:
opendir()		open a directory
readdir()		read a directory entry
closedir()		close a directory
rewinddir()		reset directory
getcwd()		get current working directory
chdir()			change current working directory
stat()			obtain file information
lstat()			obtain file information
ftsat()			obtain file information
*/

int main(int argc, String* argv) {
	unsigned cwd_buffer = 4096;
	char cwd[cwd_buffer];
	getcwd(cwd,cwd_buffer);
	
	if (argc == 1) {
		ls_plain(cwd);
	} else if (argc == 2) {
		fprintf(stderr,"Error: args not yet supported.\n");
	} else {
		fprintf(stderr,"Error: multiple args not yet supported.\n");
	}
	
	return 1;
}

DIR* getDir(String path) {
	DIR* dir;
	if ((dir = opendir(path)) == NULL) {
		fprintf(stderr, "Error while opening directory: %s\n", path);
		exit(0);
	}
	return dir;
}

/**
	List the names of files and directories in the current directory.
	Note: real ls starts line wrapping with 14+ files; left-aligns columns
	@param path the full path of the directory to examine
*/
void ls_plain(String path) {
	DIR* dir = getDir(path);
	
	struct dirent* dent;	
	while ((dent = readdir(dir)) != NULL) {										//Iterate through all items in the current directory
		if (!streq(".", dent->d_name) && !streq("..", dent->d_name)) {
			printf("%s  ", dent->d_name);										//If the file name is not . or .., then print it
		}
	}
	printf("\n");
	
	if (closedir(dir) == -1) {
		fprintf(stderr,"Encountered error while closing %s\n", path);
		exit(0);
	}
}

/**
	List the names and i-node numbers of files and directories in the current
	directory.
	@param path the full path of the directory to examine
*/
void ls_i(String path) {
	DIR* dir = getDir(path);
	
	if (closedir(dir) == -1) {
		fprintf(stderr,"Encountered error while closing %s\n", path);
		exit(0);
	}
}

/**
	List the names of files and directories in long format.
	@param path the full path of the directory to examine
*/
void ls_l(String path) {
	DIR* dir = getDir(path);
	
	if (closedir(dir) == -1) {
		fprintf(stderr,"Encountered error while closing %s\n", path);
		exit(0);
	}
}

/**
	Recursively list the names of all files and directories in the current
	directory; that is, the command is also applied to the directories within
	the current directory, the directories within those directories, and so
	forth.
	@param path the full path of the directory to examine
*/
void ls_R(String path) {
	DIR* dir = getDir(path);
	
	if (closedir(dir) == -1) {
		fprintf(stderr,"Encountered error while closing %s\n", path);
		exit(0);
	}
}
