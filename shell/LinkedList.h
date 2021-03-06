/**
 * Douglas Skrypa
 * LinkedList implementation
 * 2015.02.19
 */
 
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct ListItem {
	unsigned index;
	char* value;
	struct ListItem* next;
} LItem;

typedef struct LinkedList* List;

List list_create(int size);
LItem* list_getFirst(List l);
LItem* list_getLast(List l);
void list_destroy(List l);
unsigned list_size(List l);
void list_insert(List l, unsigned idx, char* s);
char** list_array(List l);
char* list_getVal(List l, unsigned index);
char* list_getFirstVal(List l);
char* list_getLastVal(List l);
_Bool list_contains(List l, char* val);
List list_subList(List l, char* delimiter, _Bool before);

#endif
