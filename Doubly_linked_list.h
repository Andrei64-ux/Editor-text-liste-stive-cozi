#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include "Node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct DoublyLinkedList_T {
    Node *head, *tail;
    int size;
    size_t valSize;
} DoublyLinkedList;

DoublyLinkedList *initList(size_t valSize);
void push_back(DoublyLinkedList *list, void *val);
void push_front(DoublyLinkedList *list, void *val);
void pop_front(DoublyLinkedList *list);
void pop_back(DoublyLinkedList *list);
void insert(DoublyLinkedList *list, int pos, void *val);
void erase(DoublyLinkedList *list, int pos);
Node *getList(DoublyLinkedList *list, int pos);
void clearList(DoublyLinkedList *list);
void freeList(DoublyLinkedList *list);

#endif /* DOUBLYLINKEDLIST_H */
