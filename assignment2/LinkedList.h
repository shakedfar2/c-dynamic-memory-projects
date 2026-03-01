#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Defs.h"

typedef struct Node* Node; // Represents a single node in the list
typedef struct LinkedList* LinkedList; // Represents the entire linked list
LinkedList createLinkedList(CopyFunction cf,FreeFunction free_f, PrintFunction pf, EqualFunction full_comp_f, EqualFunction partial_comp_func);
status DestroyLinkedList(LinkedList linked_list);
status appendNode(LinkedList linked_list, Element elem);
status deleteNode(LinkedList linked_list, Element elem);
status DisplayList(LinkedList linked_list);
Element getDataByIndex(LinkedList linked_list, int index);
int getLengthList (LinkedList linked_list);
Element searchByKeyInList(LinkedList linked_list, Element key);

LinkedList shallowCopyLinkedList(LinkedList original_list);
#endif //LINKEDLIST_H
