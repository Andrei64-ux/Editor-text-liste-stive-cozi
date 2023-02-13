#include "Node.h"

Node *initNode(void *val, size_t valSize, Node *next, Node *prev) {

    Node *newNode = (Node *) malloc(sizeof(Node));

    newNode->val = malloc(valSize);
    memcpy(newNode->val, val, valSize);
    newNode->next = next;
    newNode->prev=prev;

    return newNode;
}
