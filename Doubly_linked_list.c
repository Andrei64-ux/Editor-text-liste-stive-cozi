#include "Doubly_linked_list.h"

DoublyLinkedList *initList(size_t valSize) {
    DoublyLinkedList *newList = (DoublyLinkedList *) malloc(sizeof(DoublyLinkedList));

    newList->head = NULL;
    newList->tail = NULL;
    newList->valSize = valSize;
    newList->size = 0;

    return newList;
}

void push_back(DoublyLinkedList *list, void *val) {
    Node *newNode = initNode(val, list->valSize, NULL , list->tail);

    if(list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
        list->size = 1;
        return;
    }

    list->tail->next=newNode;
    list->tail=newNode;
    list->size++;
}

void push_front(DoublyLinkedList *list, void *val) {
    Node *newNode = initNode(val, list->valSize, list->head , NULL);

    if(list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
        list->size = 1;
        return;
    }

    list->head->prev=newNode;
    list->head = newNode;
    list->size++;
}

void pop_front(DoublyLinkedList *list) {
    if(list->head == NULL) {
        return;
    }

    if(list->size == 1){
        free(list->head->val);
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
        list->size--;
        return;
    }
    Node *nextNode = list->head->next;
    free(list->head->val);
    free(list->head);
    list->head = nextNode;
    list->head->prev = NULL;
    list->size--;
}

void pop_back(DoublyLinkedList *list) {
    if(list->head == NULL) {
        return;
    }

    if(list->size == 1) {
        free(list->head->val);
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
        list->size = 0;
        return;
    }

    Node *prevNode = list->tail->prev;

    free(list->tail->val);
    free(list->tail);
    list->tail = prevNode;
    list->tail->next = NULL;
    list->size--;
}

void insert(DoublyLinkedList *list, int pos, void *val)
{
    if(pos >= list->size) {
        push_back(list, val);
        return;
    }

    if(pos <= 0) {
        push_front(list, val);
        return;
    }

    Node *it = list->head;
    int i;
    for(i = 1; i < pos; i++) {
        it = it->next;
    }

    Node *newNode = initNode(val, list->valSize, it->next , it);
    it->next->prev = newNode;
    it->next = newNode;
    list->size++;
}

void erase(DoublyLinkedList *list, int pos) {
    if(pos >= list->size-1) {
        //printf("[LIST] Erasing last\n");
        pop_back(list);
        return;
    }

    if(pos <= 0) {
        //printf("[LIST] Erasing first\n");
        pop_front(list);
        return;
    }

    Node *it = list->head;
    int i;
    for(i = 1; i < pos; i++) {
        it = it->next;
    }

    Node *nextNode = it->next->next;
    free(it->next->val);
    free(it->next);
    it->next = nextNode;
    nextNode->prev = it;
    list->size--;
}

Node *getList(DoublyLinkedList *list , int pos){
    if(pos < 0 || pos >= list->size)
        return NULL;
    int i;
    Node *it=list->head;
    for(i=1; i<=pos ; i++){
        it=it->next;
    }
    return it;
}

void clearList(DoublyLinkedList *list) {
    while(list->size > 0) {
        pop_front(list);
    }
}

void freeList(DoublyLinkedList *list) {
    clearList(list);
    free(list);
}
