/**
 * Douglas Skrypa
 * Version: 2015.05.04
 * Homework 6: Implementing ls
 
 DUE: Tuesday May 5th @ 11:55 PM
 
 */

#include <stdio.h>					//IO functions
#include <stdlib.h>					//Memory functions; exit()
#include <errno.h>					//Access to stderr stream
#include <dirent.h>					//Directory access

//TEMP
#include <string.h>					//String functions
#include <ctype.h>					//Character conversions and testing

#include "modernize.h"				//Defines bool & String
//#include "stringUtils.h"			//String comparison & manipulation

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

void plain(String);

int main(int argc, String* argv) {
	DIR* dir;
	struct dirent* dent;
	
	unsigned cwd_buffer = 4096;
	char cwd[cwd_buffer];
	getcwd(cwd,cwd_buffer);
	
	plain(cwd);
	return 1;
}

void plain(String dir_name) {
	DIR* dir;
	struct dirent* dent;
	
	if ((dir = opendir(dir_name)) == NULL) {
		fprintf(stderr,"Error while opening directory: %s\n", dir_name);
		exit(0);
	}
	
	//Note: real ls starts line wrapping with 14+ files; left-aligns columns
	while ((dent = readdir(dir)) != NULL) {										//Iterate through all items in the current directory
		if ((strcmp(".", dent->d_name)!=0) && (strcmp("..", dent->d_name)!=0)) {
			printf("%s  ", dent->d_name);										//If the file name is not . or .., then print it
		}
	}
	
	if (closedir(dir) == -1) {
		fprintf(stderr,"Encountered error while closing %s\n", dir_name);
		exit(0);
	}
}