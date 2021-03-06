/**
 * Douglas Skrypa
 * LinkedList implementation
 * 2015.02.19
 */
 
#include <stdio.h>					//IO functions
#include <stdlib.h>					//Memory functions
#include <string.h>					//String functions
#include <ctype.h>					//Character conversions and testing

#include "LinkedList.h"

struct LinkedList {
	LItem* first;
	LItem* last;
	unsigned size;
	int capacity;
};

void freeItem(LItem* li) {
	if (li != NULL) {
		free(li->value);
		li->next = NULL;
		free(li);
	}
}

void freeChildren(LItem* li) {
	if (li != NULL) {
		freeChildren(li->next);
		freeItem(li);
	}
}

List list_create(int maxsize) {
	List l;
	l = malloc(sizeof(struct LinkedList));
	l->first = NULL;
	l->last = NULL;
	l->size = 0;
	l->capacity = maxsize;
	return l;
}

LItem* list_getFirst(List l) {
	return l->first;
}

LItem* list_getLast(List l) {
	return l->last;
}

void list_destroy(List l) {
	if (l != NULL) {
		freeChildren(l->first);
		free(l);
	}
}

unsigned list_size(List l) {
	return l->size;
}

void list_insert(List l, unsigned idx, char* s) {
	LItem* li;
	li = malloc(sizeof(struct ListItem));
	li->index = idx;
	li->value = ((s == NULL) ? NULL : strdup(s));
	li->next = NULL;
	
	if (l->first == NULL) {
		l->first = li;
		l->size = 1;
	} else if (l->size == l->capacity) {
		LItem* first = l->first;
		l->first = first->next;
		freeItem(first);
		l->last->next = li;
	} else {
		l->last->next = li;
		l->size += 1;
	}
	l->last = li;
}

char** list_array(List l) {
	char** array;
	array = malloc(sizeof(char*) * l->size);
	unsigned i = 0;
	LItem* li;
	for (li = l->first; li != NULL; li = li->next) {
		array[i++] = li->value;
	}
	return array;
}

char* list_getVal(List l, unsigned i) {
	LItem* li;
	for (li = l->first; li != NULL; li = li->next) {
		if (li->index == i) {
			char* retval = strdup(li->value);
			return retval;
		}
	}
	return NULL;
}

char* list_getFirstVal(List l) {
	if (l->first == NULL) {
		return NULL;
	}
	char* val = strdup(l->first->value);
	return val;
}

char* list_getLastVal(List l) {
	if (l->last == NULL) {
		return NULL;
	}
	char* val = strdup(l->last->value);
	return val;
}

_Bool list_contains(List l, char* val) {
	LItem* li;
	for (li = l->first; li != NULL; li = li->next) {
		if ((li->value != NULL) && (strcasecmp(li->value, val) == 0)) {
			return 1;
		}
	}
	return 0;
}

/*
	Builds a new List that is a sublist of the given list, from the start to the
	given string.  If before, returns the portion before the given string,
	otherwise returns the portion after the given string.
*/
List list_subList(List l, char* delimiter, _Bool before) {
	List sublist = list_create(0);
	LItem* li;
	if (before) {
		for (li = l->first; li != NULL; li = li->next) {
			if ((li->value != NULL) && (strcasecmp(li->value, delimiter) == 0)) {
				break;
			} else {
				list_insert(sublist, 0, li->value);
			}
		}
	} else {
		_Bool past = 0;
		for (li = l->first; li != NULL; li = li->next) {
			if ((li->value != NULL) && (strcasecmp(li->value, delimiter) == 0)) {
				past = 1;
			} else if (past) {
				list_insert(sublist, 0, li->value);
			}
		}
	}
	list_insert(sublist, 0, NULL);
	return sublist;
}
