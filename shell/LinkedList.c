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
	li = malloc(sizeof(LItem));
	li->index = idx;
	li->value = s == NULL ? NULL : strdup(s);
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

char* list_get(unsigned i) {
	LItem* li;
	for (li = l->first; li != NULL; li = li->next) {
		if (li->index == i) {
			char* retval = strdup(li->value);
			return retval;
		}
	}
	return NULL;
}
