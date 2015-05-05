/**
 * Douglas Skrypa
 * Version: 2015.05.04
 * Homework 6: Implementing ls
 */

#include <unistd.h>					//Misc symbolic constants, types, and functions
#include <stdio.h>					//IO functions
#include <stdlib.h>					//Memory functions; exit()
#include <sys/stat.h>				//File information
#include <sys/types.h>				//Misc type declarations
#include <string.h>					//String functions
#include <errno.h>					//Access to stderr stream
#include <dirent.h>					//Directory access
#include <time.h>					//Time formatting
#include <pwd.h>					//User info
#include <grp.h>					//Group info

#include "modernize.h"				//Defines bool & String
#include "stringUtils.h"			//String comparison & manipulation

#include "myls.h"

int main(int argc, String* argv) {
	unsigned cwd_buffer = 4096;
	char cwd[cwd_buffer];
	getcwd(cwd,cwd_buffer);
	
	if (argc == 1) {
		ls_plain(cwd);
	} else if (argc == 2) {
		if (streq("-i", argv[1])) {
			ls_i(cwd);
		} else if (streq("-l", argv[1])) {
			ls_l(cwd);
		} else if (streq("-R", argv[1])) {
			ls_R(cwd, true, NULL);
		} else {
			fprintf(stderr,"Error: Invalid argument for %s: %s\n", argv[0], argv[1]);
			return 0;
		}
	} else {
		fprintf(stderr,"Error: Invalid argument for %s\n", argv[0]);
		return 0;
	}
	
	return 1;
}

/**
	Get a DIR pointer for the given path
	@param path the path to the desired directory
	@return the DIR pointer for the given path
*/
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
	
	unsigned long fc = count_files(path);
	String* farr = malloc(sizeof(String) * fc);									//Allocate memory for the file/dir name arrays
	
	int i = 0;
	struct dirent* dent;	
	while ((dent = readdir(dir)) != NULL) {										//Iterate through all items in the current directory
		if (!streq(".", dent->d_name) && !streq("..", dent->d_name)) {			//Store if file name is not "." or ".."
			farr[i++] = strdup(dent->d_name);
		}
	}
	
	if (fc > 0) {
		sort_arr(fc, farr);														//Sort the list of file names
		for (i = 0; i < fc; i++) {												//Then print them
			printf("%s  ", farr[i]);
		}
	}
	free_arr(fc, farr);
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
	
	unsigned long fc = count_files(path);
	String* farr = malloc(sizeof(String) * fc);									//Allocate memory for the file/dir name arrays
	
	int i = 0;
	struct dirent* dent;
	while ((dent = readdir(dir)) != NULL) {										//Iterate through all items in the current directory
		if (!streq(".", dent->d_name) && !streq("..", dent->d_name)) {			//Store if file name is not "." or ".."
			farr[i++] = strdup(dent->d_name);
		}
	}
	
	if (fc > 0) {
		sort_arr(fc, farr);														//Sort the list of file names
		for (i = 0; i < fc; i++) {												//Then print them
			String fname = concat(3, path, "/", farr[i]);						//Generate the full path
			struct stat sb;
			if (stat(fname, &sb) == -1) {										//Retrieve the file's stat info
				fprintf(stderr, "Error: Unable to stat %s\n", fname);			//Print error and exit on error
				free(fname);
				exit(0);
			}
			free(fname);
			
			printf("%ld %s  ", (long) sb.st_ino, farr[i]);
			if ((i+1) % 5 == 0) {
				printf("\n");
			}
		}
	}
	free_arr(fc, farr);
	printf("\n");
	
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
	
	unsigned long fc = count_files(path);
	String* arr_a = malloc(sizeof(String) * fc);								//Allocate memory for the file name arrays
	String* arr_b = malloc(sizeof(String) * fc);
	
	int i = 0;
	struct dirent* dent;
	while ((dent = readdir(dir)) != NULL) {										//Iterate through all items in the current directory
		if (!streq(".", dent->d_name) && !streq("..", dent->d_name)) {			//Print if file name is not "." or ".."
			arr_a[i] = strdup(dent->d_name);
			arr_b[i++] = concat(3, path, "/", dent->d_name);					//Generate the full path
		}
	}
	
	sort_arr(fc, arr_a);														//Sort the list of file names
	sort_arr(fc, arr_b);														//Sort the list of file names
	
	for (i = 0; i < fc; i++) {
		struct stat sb;
		if (stat(arr_b[i], &sb) == -1) {										//Retrieve the file's stat info
			fprintf(stderr, "Error: Unable to stat %s\n", arr_b[i]);			//Print error and exit on error
			free(arr_a);
			free_arr(fc, arr_b);
			exit(0);
		}
		
		String prm = getPermissions(sb.st_mode);								//Retrieve readable file permissions
		long linkc = (long) sb.st_nlink;										//Retrieve the link count
		
		long owner = (long) sb.st_uid;											//Retrieve the owner's ID
		struct passwd* upw = getpwuid(owner);									//Convert to name
		String o_n = upw->pw_name;
		
		long group = (long) sb.st_gid;											//Retrieve the group's ID
		struct group* gpw = getgrgid(group);									//Convert to name
		String g_n = gpw->gr_name;
		
		long bytes = (long) sb.st_size;											//Retrieve the item's size
		
		char lmod[BUFSIZ], month[4], clock[6];									//Retrieve the last modified date
		struct tm* mt = localtime(&sb.st_mtime);
		strftime(month, 4, "%b", mt);											//Format the date properly
		strftime(clock, 6, "%H:%M", mt);
		sprintf(lmod, "%s %2d %s", month, mt->tm_mday, clock);
		
		String fmt = "%s %ld %s %s %ld %s %s\n";								//Specify the format String for the output
		printf(fmt, prm, linkc, o_n, g_n, bytes, lmod, arr_a[i]);				//Print the formatted output
		free(prm);																//Free the permissions String
	}
	
	free_arr(fc, arr_a);														//Free the file name arrays
	free_arr(fc, arr_b);
	
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
	@param first true if this is the first call to ls_R
	@param header header to use for printing the directory contents
*/
void ls_R(String path, bool first, String header) {
	DIR* dir = getDir(path);
	
	unsigned long dc = count_dirs(path);
	unsigned long fc = count_files(path);
	
	String* farr = malloc(sizeof(String) * fc);									//Allocate memory for the file/dir name arrays
	String* darr = malloc(sizeof(String) * dc);
	
	int i = 0, d = 0;
	struct dirent* dent;
	while ((dent = readdir(dir)) != NULL) {										//Iterate through all items in the current directory
		if (!streq(".", dent->d_name) && !streq("..", dent->d_name)) {			//Print if file name is not "." or ".."
			String fname = concat(3, path, "/", dent->d_name);					//Generate the full path
			
			struct stat sb;
			if (stat(fname, &sb) == -1) {										//Retrieve the file's stat info
				fprintf(stderr, "Error: Unable to stat %s\n", fname);			//Print error and exit on error
				free(fname);
				exit(0);
			}
			
			if (S_ISDIR(sb.st_mode)) {
				darr[d++] = strdup(dent->d_name);
			}
			farr[i++] = strdup(dent->d_name);
			free(fname);
		}
	}
	
	if (first) {																//Print the header
		printf(".:\n");
	} else {
		printf("\n%s:\n", header);
	}
	if (fc > 0) {
		sort_arr(fc, farr);														//Sort the list of file names
		for (i = 0; i < fc; i++) {
			printf("%s  ", farr[i]);
		}
	}
	free_arr(fc, farr);
	printf("\n");
	
	if (dc > 0) {
		sort_arr(dc, darr);														//Sort the list of dir names
		for (i = 0; i < dc; i++) {
			String fname = concat(3, path, "/", darr[i]);						//Generate the full path
			
			String hdr;															//Generate the header
			if (first) {
				hdr = concat(2, "./", darr[i]);
			} else {
				hdr = concat(3, header, "/", darr[i]);
			}
			
			ls_R(fname, false, hdr);											//Recursive call for subfolder
			free(fname);														//Free concatenated strings
			free(hdr);
		}
	}	
	free_arr(dc, darr);
	
	if (closedir(dir) == -1) {
		fprintf(stderr,"Encountered error while closing %s\n", path);
		exit(0);
	}
}

/**
	Converts the given file permissions to a more readable form
	@param mode the raw protection mode from the file's stat
	@return a String representation of the file permissions
*/
String getPermissions(mode_t mode) {
	char p[11];
	p[0] = S_ISDIR(mode) ? 'd' : '-';
	p[1] = (mode & S_IRUSR) ? 'r' : '-';
	p[2] = (mode & S_IWUSR) ? 'w' : '-';
	p[3] = (mode & S_IXUSR) ? 'x' : '-';
	p[4] = (mode & S_IRGRP) ? 'r' : '-';
	p[5] = (mode & S_IWGRP) ? 'w' : '-';
	p[6] = (mode & S_IXGRP) ? 'x' : '-';
	p[7] = (mode & S_IROTH) ? 'r' : '-';
	p[8] = (mode & S_IWOTH) ? 'w' : '-';
	p[9] = (mode & S_IXOTH) ? 'x' : '-';
	p[10] = '\0';
	return strdup(p);
}

/**
	Counts the number of files in the given path.
	@param path the file path to examine
	@return the number of files that exist in the given directory
*/
unsigned long count_files(String path) {
	DIR* dir = getDir(path);
	
	unsigned long i = 0;
	struct dirent* dent;	
	while ((dent = readdir(dir)) != NULL) {										//Iterate through all items in the current directory
		if (!streq(".", dent->d_name) && !streq("..", dent->d_name)) {			//Count if file name is not "." or ".."
			i++;
		}
	}
	
	if (closedir(dir) == -1) {
		fprintf(stderr,"Encountered error while closing %s\n", path);
		exit(0);
	}
	
	return i;
}

/**
	Counts the number of directories in the given path.
	@param path the file path to examine
	@return the number of directories that exist in the given directory
*/
unsigned long count_dirs(String path) {
	DIR* dir = getDir(path);
	
	unsigned long i = 0;
	struct dirent* dent;	
	while ((dent = readdir(dir)) != NULL) {										//Iterate through all items in the current directory
		if (!streq(".", dent->d_name) && !streq("..", dent->d_name)) {			//Count if file name is not "." or ".."
			String fname = concat(3, path, "/", dent->d_name);					//Generate the full path
			
			struct stat sb;
			if (stat(fname, &sb) == -1) {										//Retrieve the file's stat info
				fprintf(stderr, "Error: Unable to stat %s\n", fname);			//Print error and exit on error
				free(fname);
				exit(0);
			}
			free(fname);
			
			if (S_ISDIR(sb.st_mode)) {
				i++;
			}
		}
	}
	
	if (closedir(dir) == -1) {
		fprintf(stderr,"Encountered error while closing %s\n", path);
		exit(0);
	}
	
	return i;
}
