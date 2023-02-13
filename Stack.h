#ifndef STACK_H
#define STACK_H

#include "Node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Stack_T {
    Node *bottom, *top;
    int size;
    size_t valSize;
} Stack;

Stack *initStack(size_t valSize);
void push(Stack *stack, void *val);
void pop(Stack *stack);
Node *getStack(Stack *stack, int pos);
void clear(Stack *stack);
void freeStack(Stack *stack);

#endif /* STACK_H */
