#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node_T {
    void *val;
    struct Node_T *next, *prev;
} Node;

Node *initNode(void *val, size_t valSize, Node *next, Node *prev);

#endif /* NODE_H */
