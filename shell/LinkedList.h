/**
 * Douglas Skrypa
 * LinkedList implementation
 * 2015.02.19
 */
 
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct LinkedList* List;

List list_create(int size);
void list_destroy(List l);
unsigned list_size(List l);
void list_insert(List l, unsigned idx, char* s);
char** list_array(List l);

#endif