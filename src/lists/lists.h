#ifndef LISTS_H
#define LISTS_H

#define SUCCESS 0
#define ERROR_MEMORY_ALLOCATION 1
#define ERROR_MEMORY_LEAK 2
#define ERROR_NULL_POINTER 3
#define ERROR_BUFFER_OVERFLOW 4
#define ERROR_OPEN_FILE 5

#define MAX_LISTS 10
#define MAX_PATH 256

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern const char *list_info_file;

typedef struct NODE {
    int data;
    struct NODE* next;  
} NODE;

typedef struct {
    NODE* head;
    NODE* tail;
    size_t size;
} LINKED_LIST;

typedef struct {
    char paths[MAX_LISTS][MAX_PATH];
    int count;
} ListCollection;

int create_linked_list(LINKED_LIST** link);

int create_node(NODE** node, int data);

char* load_path_list_file(int choice_list);

int load_list(int choice_list, int choice_type_insert, int data);

int load_list_info();

int insert_node_at_beginning(LINKED_LIST** list, 
                             NODE**        node, 
                             int           data);

int insert_node_at_end(LINKED_LIST** list, 
                       NODE**        node, 
                       int           data);

int insert_node(LINKED_LIST** list,
                int           data,
                int           type_insert);

int free_list(LINKED_LIST** list);

int converting_to_string(LINKED_LIST** list, char* string, size_t size_string);

int save_list(LINKED_LIST** list, char* path_to_list);

int print_file_list(int choice_list);

#endif
