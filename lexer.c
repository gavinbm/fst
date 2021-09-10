#include "tinyforth.h"

struct Token *lex(char *input) {
    // iterate over the input stream and tokenize it.
    // catch any invalid tokens and throw the appropriate error
    // if no errors in the tokens are detected, return the list of tokens

    struct Token *tokens = NULL;
    char *substr;
    int curr_pos, key;

    for(int i = 0; i < strlen(input); ++i) {

        switch(input[i]) {
            case ' ':
                break;
            case '+':
                substr = malloc(2 * sizeof(char));
                substr[0] = '+';
                substr[1] = '\0';
                create_token(&tokens, substr, 0);
                free(substr);
                break;
            case '-':
                substr = malloc(2 * sizeof(char));
                substr[0] = '-';
                substr[1] = '\0';
                create_token(&tokens, substr, 0);
                free(substr);
                break;
            case '*':
                substr = malloc(2 * sizeof(char));
                substr[0] = '*';
                substr[1] = '\0';
                create_token(&tokens, substr, 0);
                free(substr);
                break;
            case '/':
                substr = malloc(2 * sizeof(char));
                substr[0] = '/';
                substr[1] = '\0';
                create_token(&tokens, substr, 0);
                free(substr);
                break;
            case '%':
                substr = malloc(2 * sizeof(char));
                substr[0] = '%';
                substr[1] = '\0';
                create_token(&tokens, substr, 0);
                free(substr);
                break;
            case '\"':
                i++; // get our current position passed the first qutoation
                curr_pos = i; // save the position of the first character of the string
                while(input[i] != '\"') {
                    // iterate over the string checking for illegal characters
                    if(input[i] == '\n' || input[i] == '\t' || input[i] == '\r' || input[i] == '\\' || input[i] == '%') {
                        printf("illegal char in string...\n");
                        exit(1);
                    }
                    i++; // increase the iterator so we keep moving through the file
                }
                // allocate memory for and copy the contents of the string 
                substr = malloc((i - curr_pos + 1) * sizeof(char));
                memcpy(substr, &input[curr_pos], i - curr_pos + 1);
                substr[i - curr_pos] = '\0'; // set the null-terminator
                create_token(&tokens, substr, 2);
                free(substr);
                break;
            default:
                // If we're dealing with an instruction word
                if(isalpha(input[i])) {
                    curr_pos = i;
                    while(input[i] != ' ')
                        i++;
                    
                    substr = malloc((i - curr_pos + 1) * sizeof(char));
                    memcpy(substr, &input[curr_pos], i - curr_pos + 1);
                    substr[i - curr_pos] = '\0';

                    create_token(&tokens, substr, 0);
                    
                    free(substr);
                }
                // if we're dealing with an integer literal
                else if(isdigit(input[i])) {
                    // save the current position
                    curr_pos = i;
                    while(isdigit(input[i]))
                        i++;

                    if(input[i] == '.') {
                        printf("no floats allowed...\n");
                        exit(1);
                    }

                    substr = malloc((i - curr_pos + 1) * sizeof(char));
                    memcpy(substr, &input[curr_pos], i - curr_pos + 1);
                    substr[i - curr_pos] = '\0'; // set the null-terminator
                    create_token(&tokens, substr, 2);
                    free(substr);
                    i--;
                }
                // an unrecognized token
                else {
                    printf("what?\n");
                }
                break;
        }
    }

    return tokens;
}

/*
Takes a double pointer to a linked list of tokens to which we will append a new node with the 
text of source(the second arg) and a type of type(the third arg).
*/
void create_token(struct Token **head, char *source, int type) {
    struct Token **tmp = head, *add;
    int source_len = strlen(source);
    
    add = malloc(sizeof(struct Token));
    add->src = malloc(source_len + 1 * sizeof(char));
    add->type = type;
    strcpy(add->src, source);
    add->next = NULL;
    
    while(*tmp)
        tmp = &(*tmp)->next;
    
    add->next = *tmp;
    *tmp = add;
}

/*
Prints all the tokens in the given list
*/
void print_tokens(struct Token *tokens) {
    struct Token *tmp = tokens;

    while(tmp != NULL) {
        printf("%s\n", tmp->src);
        printf("[%s] -- [%d]\n", tmp->src, tmp->type);
        tmp = tmp->next;
    }
}

/*
Frees all the tokens in the given list
*/

void free_tokens(struct Token *tokens) {
    struct Token *tmp = tokens, *tmp2;

    while(tmp != NULL) {
        tmp2 = tmp;
        tmp = tmp->next;
        free(tmp2->src);
        free(tmp2);
    }
}