#ifndef MY_STRING_H
#define MY_STRING_H

#include <stdio.h>
#include "generic.h"

enum status { Failure, Success };
typedef enum status Status;
enum boolean { FALSE, TRUE };
typedef enum boolean Boolean;

typedef void* MY_STRING;

MY_STRING my_string_init_default(void);
MY_STRING my_string_init_c_string(const char* c_string);

int my_string_get_capacity(MY_STRING hMy_string);
int my_string_get_size(MY_STRING hMy_string);
int my_string_compare(MY_STRING hLeft_string, MY_STRING hRight_string);
Status my_string_extraction(MY_STRING hMy_string, FILE* fp);
Status my_string_insertion(MY_STRING hMy_string, FILE* fp);
Status my_string_push_back(MY_STRING hMy_string, char c);
Status my_string_pop_back(MY_STRING hMy_string);
Status my_string_concat(MY_STRING hResult, MY_STRING hAppend);
char* my_string_at(MY_STRING hMy_string, int index);
char* my_string_c_str(MY_STRING hMy_string);
void my_string_print(MY_STRING hMy_string);
Boolean my_string_empty(MY_STRING hMy_string);
void my_string_assignment(ITEM* pLeft, ITEM right);
void my_string_print_s(MY_STRING hMy_string);

void my_string_destroy(ITEM* pItem);

#endif