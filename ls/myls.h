/**
 * Douglas Skrypa
 * Version: 2015.05.04
 * Homework 6: Implementing ls
 */

/**
	Get a DIR pointer for the given path
	@param path the path to the desired directory
	@return the DIR pointer for the given path
*/
DIR* getDir(String path);

/**
	List the names of files and directories in the current directory.
	Note: real ls starts line wrapping with 14+ files; left-aligns columns
	@param path the full path of the directory to examine
*/
void ls_plain(String path);

/**
	List the names and i-node numbers of files and directories in the current
	directory.
	@param path the full path of the directory to examine
*/
void ls_i(String path);

/**
	List the names of files and directories in long format.
	@param path the full path of the directory to examine
*/
void ls_l(String path);

/**
	Recursively list the names of all files and directories in the current
	directory; that is, the command is also applied to the directories within
	the current directory, the directories within those directories, and so
	forth.
	@param path the full path of the directory to examine
*/
void ls_R(String path);

/**
	Converts the given file permissions to a more readable form
	@param mode the raw protection mode from the file's stat
	@return a String representation of the file permissions
*/
String getPermissions(mode_t mode);

/**
	Counts the number of files in the given path.
	@param path the file path to examine
	@return the number of files that exist in the given directory
*/
unsigned long count_files(String path);
