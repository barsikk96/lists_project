#include "lists.h"
#include "../output/output.h"

const char *path_to_list_info = {"../list_files/lists_info.txt"};

int create_linked_list(LINKED_LIST** link)
{
    int flag_error = SUCCESS;

    *link = malloc(sizeof(LINKED_LIST));
    if (*link == NULL)
        flag_error = ERROR_MEMORY_ALLOCATION;

    if (!flag_error)
    {
        (*link)->head = NULL;
        (*link)->tail = NULL;
        (*link)->size = 0;
    }

    return flag_error;
}

int create_node(NODE** node,
                int    data) {
    int flag_error = SUCCESS;

    if (node == NULL)
        flag_error = ERROR_NULL_POINTER;

    if (!flag_error)
    {
        *node = malloc(sizeof(NODE));
        if (*node == NULL)
            flag_error = ERROR_MEMORY_ALLOCATION;

        if (!flag_error)
        {
            (*node)->data = data;
            (*node)->next = NULL;
        }
    }

    return flag_error;
}

char* load_path_list_file(int choice_list) {
    FILE *file_info = fopen(path_to_list_info, "r");
    char* path_to_list = NULL;

    if(file_info) {
	char buffer[1024];
	while (fgets(buffer, sizeof(buffer), file_info)) {
	    int line_num;
	    char path[256];
	    if(sscanf(buffer, "%d. %255[^\n]", &line_num, path) == 2) {
		if(line_num == choice_list) {
		    path_to_list = strdup(path);
		}
	    }
	}
	fclose(file_info);
    }

    return path_to_list;
}

int load_list(int list_number, int choice_type_insert, int data) {
    int flag_error = SUCCESS;
    
    FILE *file_info = fopen(path_to_list_info, "r");
    char* path_to_list = load_path_list_file(list_number);
    
    if(path_to_list) {
    	LINKED_LIST* list = NULL;
    	if(create_linked_list(&list))
            flag_error = 1;
    	FILE *file_list = fopen(path_to_list, "r");
    	if (file_list && !flag_error) {
            int data_list;
            while(fscanf(file_list, "%d", &data_list) != EOF) {
                printf("Загружаем число: %d\n", data_list);  // <- Добавьте эту строку
		insert_node(list, data_list, 2); // подгружаем список 
            }

	    insert_node(list, data, choice_type_insert);
	    
	    save_list(&list, path_to_list);

	    free_list(&list);
	    fclose(file_list);
        }
        free(path_to_list);
    } 

    return flag_error;
}

int load_list_info() {
    int flag_error = SUCCESS;
    
    

    FILE *file = fopen(path_to_list_info, "r");
    if (file) {
        ListCollection* lists = malloc(sizeof(ListCollection));
        lists->count = 0;
        
        while (fscanf(file, "%255s", lists->paths[lists->count]) == 1 && lists->count < MAX_LISTS-1) {
            printf("%d. %s\n", lists->count + 1, lists->paths[lists->count]);
            lists->count++;
        }
        free(lists);
        
        fclose(file);
    } else {
        flag_error = ERROR_OPEN_FILE;
    }
    return flag_error;
}

int insert_node_at_beginning(LINKED_LIST** list,
                             NODE**        node,
                             int           data)
{
    int flag_error = SUCCESS;

    if (*node != NULL)
        flag_error = ERROR_MEMORY_LEAK;

    if (list == NULL || *list == NULL)
        flag_error = ERROR_NULL_POINTER;

    if (!flag_error)
    {
        if (create_node(node, data))
            flag_error = ERROR_MEMORY_ALLOCATION;

        if (!flag_error)
        {
            if ((*list)->head == NULL)
                (*list)->tail = *node;

            (*node)->next = (*list)->head;
            (*list)->head = *node;
            (*list)->size++;
        }
    }

    return flag_error;
}

int insert_node_at_end(LINKED_LIST** list,
                       NODE**        node,
                       int           data)
{
    int flag_error = SUCCESS;

    if (*node != NULL)
        flag_error = ERROR_MEMORY_LEAK;

    if (list == NULL || *list == NULL)
        flag_error = ERROR_NULL_POINTER;

    if (!flag_error)
    {
        if (create_node(node, data))
            flag_error = ERROR_MEMORY_ALLOCATION;

        if (!flag_error)
        {
            if ((*list)->head == NULL)
            {
                (*list)->head = *node;
                (*list)->tail = *node;
                (*list)->size++;
            }
            else
            {
                NODE *current_tail = (*list)->tail;
                current_tail->next = *node;
                (*list)->tail = *node;
                (*list)->size++;
            }
        }
    }

    return flag_error;
}

int insert_node(LINKED_LIST** list,
                int           data,
                int           type_insert) {
    int flag_error = SUCCESS;
    printf("Добавляем элемент %d (тип вставки: %d)\n", data, type_insert);  // <- Отладочный вывод
    NODE* node = NULL;
    switch(type_insert) {
        case 1:
            flag_error = insert_node_at_beginning(list, &node, data);
            break;
        case 2:
            flag_error = insert_node_at_end(list, &node, data);
            break;
        default:
            break;        
    }
}

int free_list(LINKED_LIST** list)
{
    int flag_error = SUCCESS;

    if (list == NULL || *list == NULL)
        flag_error = ERROR_NULL_POINTER;

    if (!flag_error)
    {
        NODE* current = (*list)->head;
        while (current != NULL)
        {
            NODE* temp = current->next;
            free(current);
            current = temp;
        }
        free(*list);
        *list = NULL;
    }

    return flag_error;
}

int converting_to_string(LINKED_LIST** list, char* string, size_t size_string) {
    int flag_error = SUCCESS;

    if (list == NULL || *list == NULL || string == NULL)
        flag_error = ERROR_NULL_POINTER;
    if (size_string == 0)
        flag_error = ERROR_BUFFER_OVERFLOW;

    NODE* current     = (*list)->head;
    char* current_pos = string;
    size_t remaining  = size_string;

    string[0] = '\0';

    while(current    != NULL &&
          flag_error == SUCCESS) {
	printf("Текущий узел: %d\n", current->data);  // Перед snprintf
	int written = snprintf(current_pos, remaining, "%d ", current->data);

        if (written < 0 || (size_t)written >= remaining) {
          flag_error = ERROR_BUFFER_OVERFLOW; // Переполнение буфера
        }

        if (flag_error == SUCCESS) {
            current_pos += written;
            remaining   -= written;
            current      = current->next;
        }
    }
    
    if (flag_error == SUCCESS && current_pos > string) {
          *(current_pos - 1) = '\0';
    }

    return flag_error;
}

int save_list(LINKED_LIST** list, char* path_to_list) {
    int flag_error = SUCCESS;
    if(list == NULL || *list == NULL || path_to_list == NULL)
	flag_error = 1;

    if ((*list)->size == 0)
        printf("Список пуст! Нечего сохранять.\n");

    if(flag_error == SUCCESS) {
        size_t result_size = (*list)->size * 20 + 1;
        char* result = (char*)malloc(result_size);
        if(result == NULL) 
            flag_error = 1;
    

    	if(flag_error == SUCCESS)
            flag_error = converting_to_string(list, result, result_size);
	
	printf("Данные для сохранения: '%s'\n", result);
	save_to_file(result, path_to_list);

    	free(result);
    }

    return flag_error;
}

int print_file_list(int choice_list) {
    int flag_error = 0;
    
    char* path_to_list = load_path_list_file(choice_list);

    if(path_to_list != NULL) {
    	FILE* file = fopen(path_to_list, "r");
        if(file) {
	    char buffer[256];
	    if(fgets(buffer, sizeof(buffer), file) != NULL) {
	    	buffer[strcspn(buffer, "\n")] = '\0';
	    	printf("%s\n", buffer);
	    }
	    fclose(file);
    	} else {
	    flag_error = 1;
    	}
    	free(path_to_list);
    }
}
