#include "my_string.h"
#include <stdlib.h>
#include <stdio.h>

struct my_string {
    int size;
    int capacity;
    char* data;
};
typedef struct my_string My_string;

MY_STRING my_string_init_default(void) {
    My_string* pMy_string = (My_string*)malloc(sizeof(My_string));

    if (pMy_string != NULL) {
        pMy_string->size = 0;
        pMy_string->capacity = 7;
        pMy_string->data = malloc(sizeof(char) * (pMy_string->capacity + 1));
        if (pMy_string->data == NULL) {
            printf("Failed to allocate space for data");
            free(pMy_string);
            return NULL;
        }
        pMy_string->data[0] = '\0';
    }

    return (MY_STRING)pMy_string;
}


void my_string_destroy(ITEM* pItem) {
    My_string* pMy_string = (My_string*)*pItem;
    
    free(pMy_string->data);                                                                                                 
    pMy_string->data = NULL;
    
    free(pMy_string);                                                                                                       
    *pItem = NULL;
}

MY_STRING my_string_init_c_string(const char* c_string) {
    int temp = 0;

    for (int i = 0; c_string[i]; i++) { 
        temp++; 
    }

    My_string* pMy_string = (My_string*)malloc(sizeof(My_string));

    if (pMy_string != NULL) {
        pMy_string->size = temp;
        pMy_string->capacity = temp;
        pMy_string->data = malloc(sizeof(char) * (pMy_string->capacity + 1));

        if (pMy_string->data == NULL) {
            free(pMy_string);
            return NULL;
        }
        else {
            for (int i = 0; i < pMy_string->capacity; i++) {
                pMy_string->data[i] = c_string[i];
            }
            pMy_string->data[pMy_string->capacity] = '\0';
        }
    }
    return (MY_STRING) pMy_string;
}

int my_string_get_capacity(MY_STRING hMy_string) {
    My_string* pMy_string = (My_string*)hMy_string;

    return pMy_string->capacity;
}

int my_string_get_size(MY_STRING hMy_string) {
    My_string* pMy_string = (My_string*)hMy_string;

    return pMy_string->size;
}

int my_string_compare(MY_STRING hLeft_string, MY_STRING hRight_string) {
    My_string* pRight_string = (My_string*)hRight_string;
    My_string* pLeft_string = (My_string*)hLeft_string;

    int size = pLeft_string->size;
    int temp = 0;
    for (int i = 0; i < (size + 1); i++) {
        temp = pLeft_string->data[i] - pRight_string->data[i];

        if (temp != 0) {
            return temp;
        }
    }
    
    return temp;
}

Status my_string_extraction(MY_STRING hMy_string, FILE* fp) {
    My_string* pMy_string = (My_string*)hMy_string;
    char c;
    int noc;

    pMy_string->size = 0;

    noc = fscanf(fp, " %c", &c);
    while (noc > 0 && c != ' ' && c != '\n') {
        my_string_push_back((MY_STRING)pMy_string, c);
        noc = fscanf(fp, "%c", &c);
    }

    if (noc != 1 && pMy_string->size < 1) {
        return Failure;
    }

    ungetc(c, fp);

    return Success;
}

Status my_string_insertion(MY_STRING hMy_string, FILE* fp) {
    My_string* pMy_string = (My_string*)hMy_string;
    int noc;

    for (int i = 0; i < pMy_string->size; i++) {
        noc = fprintf(fp, "%c", pMy_string->data[i]);

        if (noc != 1) {
            return Failure;
        }
    }

    return Success;
}

Status my_string_push_back(MY_STRING hMy_string, char c) {
    My_string* pMy_string = (My_string*)hMy_string;
    int i;

    if (pMy_string->size >= pMy_string->capacity) {
        char* temp = (char*)malloc(sizeof(char) * ((pMy_string->capacity * 2) + 1) );

        if (temp == NULL) {
            return Failure;
        }
        for (i = 0; i < pMy_string->size; i++) {
            temp[i] = pMy_string->data[i];
        }

        pMy_string->capacity *= 2;
        free(pMy_string->data);
        pMy_string->data = temp;
    }

    pMy_string->data[pMy_string->size] = c;
    pMy_string->size++;
    pMy_string->data[pMy_string->size] = '\0';

    return Success;
}
Status my_string_pop_back(MY_STRING hMy_string) {
    My_string* pMy_string = (My_string*)hMy_string;

    if (pMy_string->size > 0) {
        pMy_string->size--;
        return Success;
    }

    return Failure;
}

char* my_string_at(MY_STRING hMy_string, int index) {
    My_string* pMy_string = (My_string*)hMy_string;

    if ((pMy_string->size - 1) < index) {
        return NULL;
    }

    return &pMy_string->data[index];
}

char* my_string_c_str(MY_STRING hMy_string) {
    My_string* pMy_string = (My_string*)hMy_string;

    return pMy_string->data;
}

Boolean my_string_empty(MY_STRING hMy_string) {
    My_string* pMy_string = (My_string*)hMy_string;

    if (pMy_string->size == 0) {
        return TRUE;
    }
    return FALSE;
}

Status my_string_concat(MY_STRING hResult, MY_STRING hAppend) {
    My_string* pResult = (My_string*)hResult;
    My_string* pAppend = (My_string*)hAppend;
    Status status;

    for (int i = 0; i < pAppend->size; i++) {
        status = my_string_push_back(pResult, pAppend->data[i]);
        if (status == Failure) {
            return Failure;
        }
    }

    return Success;
}

void my_string_assignment(ITEM* pLeft, ITEM right) {
    My_string* Right = (My_string*)right;
    
    if (*pLeft == NULL) {
        *pLeft = my_string_init_c_string(Right->data);
    }
    else {
        while (my_string_pop_back(*pLeft));
        for (int i = 0; i < my_string_get_size((MY_STRING)Right); i++) {
            my_string_push_back((MY_STRING)*pLeft, Right->data[i]);
        }

    }
}

void my_string_print(MY_STRING hMy_string) {
    My_string* pMy_string = (My_string*)hMy_string;

    for (int i = 0; i < pMy_string->size; i++) {
        printf("%c ", pMy_string->data[i]);
    }
    return;
}

void my_string_print_s(MY_STRING hMy_string) {
    My_string* pMy_string = (My_string*)hMy_string;

    for (int i = 0; i < pMy_string->size; i++) {
        printf("%c", pMy_string->data[i]);
    }
    printf("\n");
    return;
}