#include "ibp.h"

void create_entry(struct DictEntry **head, char *name, char *instructions) {
    struct DictEntry **tmp = head, *add;
    int name_len = strlen(name) ins_len = strlen(instructions);
    
    add = malloc(sizeof(struct DictEntry));
    add->word = malloc(name_len + 1 * sizeof(char));
    add->instructions = malloc(ins_len + 1 * sizeof(char));
    strcpy(add->word, name);
    strcpy(add->instructions, instructions);
    add->next = NULL;
    
    while(*tmp)
        tmp = &(*tmp)->next;
    
    add->next = *tmp;
    *tmp = add;
}