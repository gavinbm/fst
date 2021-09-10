#include "tinyforth.h"

/*
    Initialize the stack that we'll push things to.
*/
struct Stack *init_stack() {
    struct Stack *stack = malloc(sizeof(struct Stack));
    stack->head = NULL;
    stack->len = 0;
    stack->capacity = 5;

    return stack;
}

/*
    Push a value onto the stack. Our stack is a just a linked list of values inside of another struct
    and since we pass in a pointer to the stack and manipulate the head pointer of the value list
    through that we essentially have a double pointer. This allows use to manipulate our linked list
    of values (called StackNodes) without return values or explicit double pointers.
*/
void push(struct Stack *stack, int new_value) {

    // we'll add a node if we're not at capacity
    if(stack->len < stack->capacity) {
        // init the new node
        struct StackNode *add = malloc(sizeof(struct StackNode));
        add->value = new_value;
        add->next = NULL;

        // the stack is empty
        if(stack->len == 0) {
            stack->head = add;
        }
        // the stack is not empty 
        else {

            add->next = stack->head;
            stack->head = add;
        }
        stack->len++;
    }
    // the stack is full
    else {
        printf("STACK OVERFLOW: stack at capacity, push failed, pop values before pushing...\n");
    }
}

/*
    Pop a value off our list of values. same double pointer trick going on here as in push, hence the lack of
    a return value.
*/
void pop(struct Stack *stack) {
    struct StackNode *tmp = stack->head;

    if(tmp != NULL) {
        if(tmp->next == NULL) {
            free(tmp);
            stack->head = NULL;
        } else {
            stack->head = stack->head->next;
            free(tmp);
        }
        stack->len--;
    } else {
        printf("STACK UNDERFLOW: pop failed, stack is already empty...\n");
    }
}

/*
    print all the values on the given stack.
*/
void print_stack(struct Stack *stack) {
    struct StackNode *tmp = stack->head;
    
    printf("Length: %d\n", stack->len);
    printf("Values: ");
    while(tmp != NULL) {
        printf("%d ", tmp->value);
        tmp = tmp->next;
    }
    printf("\n");
}

/*
    Free the given stack to avoid leaks.
*/
void free_stack(struct Stack *stack) {
    struct StackNode *tmp = stack->head, *tmp2;

    while(tmp != NULL) {
        tmp2 = tmp;
        tmp = tmp->next;
        free(tmp2);
    }

    free(stack);
}