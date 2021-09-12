#include "ibp.h"

void create_entry(struct DictEntry **head, char *name, char *instructions) {
    struct DictEntry **tmp = head, *add;
    int name_len = strlen(name), ins_len = strlen(instructions);
    
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

struct DictEntry *indict(struct DictEntry *head, char *name) {
    struct DictEntry *tmp = head;

    while(tmp != NULL) {
        if(strcmp(name, tmp->word) == 0)
            return tmp;
    }

    return NULL;
}

void print_dict(struct DictEntry *dict) {
    struct DictEntry *tmp = dict;

    while(tmp != NULL) {
        printf("[%s] -- [%s]\n", tmp->word, tmp->instructions);
        tmp = tmp->next;
    }
}

void free_dict(struct DictEntry *dict) {
    struct DictEntry *tmp = dict, *tmp2;

    while(tmp != NULL) {
        tmp2 = tmp;
        tmp = tmp->next;
        free(tmp2->word);
        free(tmp2->instructions);
        free(tmp2);
    }
}