/**
 * Douglas Skrypa
 * Version: 2015.04.14
 * A collection of various utilities for working with strings
 */

#include <string.h>					//String functions
#include <ctype.h>					//Character conversions and testing
#include <stdarg.h>					//Allow variable number of arguments

#include "modernize.h"				//Defines bool & String

#include "stringUtils.h"

/**
	Concatenates multiple Strings
	@param argc the number of strings that will be concatenated
	@return the result of adding each subsequent string to the right of the
	previous string
*/
String concat(int argc, ...) {
	va_list args;
	int len = 0;
	String arg[argc];
	
	va_start(args, argc);				//Initialize the variable number of args
	int i;
	for (i = 0; i < argc; i++) {
		arg[i] = va_arg(args, String);	//Store the argument
		len += strlen(arg[i]);			//Increment the length of the new String
	}
	va_end(args);						//Terminate the variable number of args
	
	char sb[len];						//Init a char array to fit the string
	strcpy(sb, arg[0]);					//Copy the first element in (important!)
	for (i = 1; i < argc; i++) {
		strcat(sb, arg[i]);				//Use strcat for the rest
	}
	
	return strdup(sb);					//Duplicate & return the new pointer
}

/**
	Compares two strings
	@param a a string
	@param b a string
	@return true if they are the same, otherwise false
*/
bool streq(String a, String b) {
	return strcmp(a, b)==0;
}

/**
	Compares two strings, ignoring case
	@param a a string
	@param b a string
	@return true if they are the same, otherwise false
*/
bool streqic(String a, String b) {
	return strcasecmp(a, b)==0;
}

/**
	Remove leading & trailing spaces from a string
	@param s a string
	@return the given string, with leading & trailing spaces removed
*/
String strip(String s){
	if(s == NULL){							//Prevent errors
		return s;
	}
	int lead = 0, trail = 0;
	char* c = s;							//Focus pointer on first char
	while(*c == ' '){						//Look at beginning for spaces
		lead++;
		c++;
	}
	c = s+strlen(s)-1;						//Focus pointer on last char
	while(*c == ' '){						//Look at end for spaces
		trail++;
		c--;
	}
	return substring(s, lead, -trail);		//Use the # of each for a substring
}

/**
	Extract a substring from a given string.  Behaves similarly to python's
	string[from:to] substring notation.
	@param s the original string
	@param from the index of the first character to include
	@param to the index of the last character to include
	@return the portion of s between from and to
*/
String substring(String s, int from, int to){
	if(s == NULL){							//Prevent errors
		return s;
	}
	int firstChar, lastChar, len;
	
	if(from < 0){							//From the end, backwards
		firstChar = strlen(s) + from;
	} else if(from >= strlen(s)){			//Outside of string bounds
		return NULL;
	} else {								//From user provided index
		firstChar = from;
	}
	
	if(to == 0){							//To the end
		lastChar = strlen(s);
	} else if(to < 0){						//To x chars before the end
		lastChar = strlen(s) + to;
	} else if(to > (strlen(s))){			//Preserve original string end
		lastChar = strlen(s);
	} else {								//To user provided index
		lastChar = to;
	}

	len = lastChar - firstChar;
	char building[len+1], *p;				//Prepare a new char array
	building[len] = '\0';
	p = building;
	
	int i;
	for(i = firstChar; i < lastChar; i++){
		*p++ = s[i];						//Populate the new char array
	}
	
	String news = strdup(building);
	return news;							//Return the new string
}
