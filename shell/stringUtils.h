/**
 * Douglas Skrypa
 * Version: 2015.02.19
 * A collection of various utilities for working with strings
 */

#ifndef STRINGUTILS_H
#define STRINGUTILS_H

typedef char* string;				//Define the string type for ease of use

/**
	Compares two strings
	@param a a string
	@param b a string
	@return true if they are the same, otherwise false
*/
_Bool streq(string a, string b);

/**
	Compares two strings, ignoring case
	@param a a string
	@param b a string
	@return true if they are the same, otherwise false
*/
_Bool streqic(string a, string b);

/**
	Remove leading & trailing spaces from a string
	@param s a string
	@return the given string, with leading & trailing spaces removed
*/
string strip(string s);

/**
	Extract a substring from a given string.  Behaves similarly to python's
	string[from:to] substring notation.
	@param s the original string
	@param from the index of the first character to include
	@param to the index of the last character to include
	@return the portion of s between from and to
*/
string substring(string s, int from, int to);

#endif
