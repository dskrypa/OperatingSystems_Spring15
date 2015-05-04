/**
 * Douglas Skrypa
 * Version: 2015.05.04
 * A collection of various utilities for working with strings
 */

#ifndef STRINGUTILS_H
#define STRINGUTILS_H

/**
	Concatenates multiple Strings
	@param argc the number of strings that will be concatenated
	@return the result of adding each subsequent string to the right of the
	previous string
*/
String concat(int argc, ...);

/**
	Compares two strings
	@param a a string
	@param b a string
	@return true if they are the same, otherwise false
*/
bool streq(String a, String b);

/**
	Compares two strings, ignoring case
	@param a a string
	@param b a string
	@return true if they are the same, otherwise false
*/
bool streqic(String a, String b);

/**
	Remove leading & trailing spaces from a string
	@param s a string
	@return the given string, with leading & trailing spaces removed
*/
String strip(String s);

/**
	Extract a substring from a given string.  Behaves similarly to python's
	string[from:to] substring notation.
	@param s the original string
	@param from the index of the first character to include
	@param to the index of the last character to include
	@return the portion of s between from and to
*/
String substring(String s, int from, int to);

#endif
