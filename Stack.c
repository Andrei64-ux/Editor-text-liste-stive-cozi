#include "Stack.h"

Stack *initStack(size_t valSize) {
    Stack *newStack = (Stack *)malloc(sizeof(Stack));

    newStack->bottom = NULL;
    newStack->top = NULL;
    newStack->valSize = valSize;
    newStack->size = 0;

    return newStack;
}

void push(Stack *stack, void *val) {
    Node *newNode = initNode(val, stack->valSize, NULL , stack->top);

    if(stack->bottom == NULL) {
        stack->bottom = newNode;
        stack->top = newNode;
        stack->size = 1;
        return;
    }

    stack->top->next=newNode;
    stack->top=newNode;
    stack->size++;
}

void pop(Stack *stack) {
    if(stack->bottom == NULL) {
        return;
    }

    if(stack->size == 1) {
        free(stack->bottom->val);
        free(stack->bottom);
        stack->bottom = NULL;
        stack->top = NULL;
        stack->size = 0;
        return;
    }

    Node *prevNode = stack->top->prev;

    free(stack->top->val);
    free(stack->top);
    stack->top = prevNode;
    stack->top->next = NULL;
    stack->size--;
}

Node *getStack(Stack *stack , int pos){
    if(pos < 0 || pos >= stack->size)
        return NULL;
    Node *it=stack->bottom;
    int i;
    for( i=1; i<=pos ; i++){
        it=it->next;
    }
    return it;
}

void clear(Stack *stack)
{
    while(stack->size > 0)
    {
        pop(stack);
    }
}

void freeStack(Stack *stack) {
    clear(stack);
    free(stack);
}
