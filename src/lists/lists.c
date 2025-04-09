#include "lists.h"
#include "../output/output.h"

const char *path_to_list_info = {"../list_files/lists_info.txt"};

int create_linked_list(LINKED_LIST** link)
{
    int flag_error = SUCCESS;

    *link = malloc(sizeof(LINKED_LIST));
    if(*link == NULL)
        flag_error = ERROR_MEMORY_ALLOCATION;

    if(!flag_error)
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

    if(node == NULL)
        flag_error = ERROR_NULL_POINTER;

    if(!flag_error)
    {
        *node     = malloc(sizeof(NODE));
        if(*node == NULL)
            flag_error = ERROR_MEMORY_ALLOCATION;

        if(flag_error == SUCCESS)
        {
            (*node)->data = data;
            (*node)->next = NULL;
        }
    }

    return flag_error;
}

int create_list_file() {
    int flag_error   = SUCCESS;
    int max_list_num = 0;

    FILE* file_info = fopen(path_to_list_info, "a+");
    if(file_info   == NULL)
	flag_error  = ERROR_OPEN_FILE;

    if(flag_error == SUCCESS) {
        char buffer[1024];
        
	while(fgets(buffer, sizeof(buffer), file_info)) {
            int current_num = 0;
            
	    if(sscanf(buffer, "%d%*[^\n]", &current_num) == 1) {
                if(current_num > max_list_num) {
                    max_list_num 	  = current_num;
                }
            }
        }
        
        int new_list_num = max_list_num + 1;
	char path[275];
	char file_name[255];
	scanf("%254s", file_name);
        
	snprintf(path, sizeof(path), "../list_files/%s.txt", file_name);

	if(access(path, F_OK) == 0) {
            fclose(file_info);
            flag_error = ERROR_FILE_EXISTS;  
	}
        
	if(flag_error == SUCCESS) {
            FILE* new_list = fopen(path, "a");
	    if(	  new_list == NULL)
		flag_error  = ERROR_OPEN_FILE;
            
	    if(flag_error == SUCCESS) {
            	fclose(new_list);
            	fprintf(file_info, 
			"%d. ../list_files/%s.txt\n", new_list_num, file_name);
            }
        
            fclose(file_info);
	}
    }
    
    return flag_error;
}

char* load_path_list_file(int choice_list) {
    int flag_error = SUCCESS;

    FILE *file_info = fopen(path_to_list_info, "r");
    if(   file_info == NULL)
	flag_error   = ERROR_OPEN_FILE;
    
    char* path_to_list = NULL;

    if(flag_error == SUCCESS) {
	char buffer[1024];
	while(fgets(buffer, sizeof(buffer), file_info)) {
	    int  line_num;
	    char path[256];
	    if(sscanf(buffer,
		      "%d. %255[^\n]", &line_num, path) == 2) {
		if(line_num == choice_list) {
		    path_to_list = strdup(path);
		}
	    }
	}
	fclose(file_info);
    }

    return path_to_list;
}

LINKED_LIST* load_list(int list_number) {
    int flag_error = SUCCESS;

    LINKED_LIST* list = NULL;
    
    FILE *file_info = fopen(path_to_list_info, "r");
    if(   file_info == NULL)
	flag_error = ERROR_OPEN_FILE;
    
    char* path_to_list = load_path_list_file(list_number);
    if(   path_to_list == NULL)
	flag_error = ERROR_MEMORY_ALLOCATION;
    
    if(flag_error == SUCCESS) {
    	flag_error = create_linked_list(&list);

    	FILE *file_list = fopen(path_to_list, "r");
	if(   file_list == NULL)
	    flag_error = ERROR_OPEN_FILE;

    	if (flag_error == SUCCESS) {
            int data_list;
            while(fscanf(file_list, "%d", &data_list) != EOF
		  			&& flag_error == SUCCESS) {
		flag_error = action_with_node(&list, 
					       data_list, 
					       2); 
            }
	    fclose(file_list);
        }
        free(path_to_list);
    } 

    return list;
}

int load_list_info() {
    int flag_error = SUCCESS;
    
    FILE *file = fopen(path_to_list_info, "r");
    if(   file == NULL)
	flag_error = ERROR_OPEN_FILE;

    if (flag_error == SUCCESS) {
        char buffer[256];
        
        while (fgets(buffer, sizeof(buffer), file)) {
            buffer[strcspn(buffer, "\n")] = '\0';
            
            printf("%s\n", buffer);
        }
        
        fclose(file);
    }

    return flag_error;
}

int insert_node_at_beginning(LINKED_LIST** list,
                             NODE**        node,
                             int           data)
{
    int flag_error  = SUCCESS;

    if( *node != NULL)
        flag_error  = ERROR_MEMORY_LEAK;

    if( list == NULL || *list == NULL)
        flag_error  = ERROR_NULL_POINTER;

    if( flag_error == SUCCESS){
        flag_error  = create_node(node, data);

        if(flag_error == SUCCESS) {
            if( (*list)->head == NULL)
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
                       int           data) {
    int flag_error  = SUCCESS;

    if( *node != NULL)
        flag_error  = ERROR_MEMORY_LEAK;

    if( list == NULL || *list == NULL)
        flag_error  = ERROR_NULL_POINTER;

    if( flag_error == SUCCESS) {
        flag_error  = create_node(node, data);

        if(flag_error == SUCCESS) {
            if( (*list)->head == NULL) {
                (*list)->head = *node;
                (*list)->tail = *node;
                (*list)->size++;
            } else {
                NODE *current_tail = (*list)->tail;
                current_tail->next = *node;
                (*list)->tail = *node;
                (*list)->size++;
            }
        }
    }

    return flag_error;
}

int action_main(int list_number,
		int data,
		int type_action) {
    int flag_error = SUCCESS;

    LINKED_LIST* list = load_list(list_number);
    if(list == NULL) {
	flag_error = ERROR_MEMORY_ALLOCATION;
    }

    char* path_to_list = load_path_list_file(list_number);
    if(   path_to_list == NULL)
	flag_error = ERROR_MEMORY_ALLOCATION;
 

    flag_error = action_with_node(&list, 
		    		   data, 
				   type_action);
    
    if(flag_error == SUCCESS) {
	save_list(&list, path_to_list);
	free(path_to_list);
    }

    if(flag_error != ERROR_MEMORY_ALLOCATION)
    	free_list(&list);

    return flag_error;
}

int action_with_node(LINKED_LIST** list,
                     int           data,
                     int           type_action) {
    int flag_error = SUCCESS;

    NODE* node = NULL;
    switch(type_action) {
        case 1:
            flag_error = insert_node_at_beginning(list, 
			    			 &node, 
						  data);
            break;
        case 2:
            flag_error =       insert_node_at_end(list, 
			    			 &node, 
						  data);
            break;
	case 3:
	    flag_error =     delete_node_by_value(list,
			    			  data);
        default:
            break;        
    }

    return flag_error;
}

int free_list(LINKED_LIST** list) {
    int flag_error = SUCCESS;

    if( list == NULL || *list == NULL)
        flag_error = ERROR_NULL_POINTER;

    if( flag_error == SUCCESS) {
        NODE* current  = (*list)->head;
        
	while(current != NULL) {
            NODE*     temp = current->next;
            free(current);
            current = temp;
        }
        free(*list);
        *list = NULL;
    }

    return flag_error;
}

int delete_list_file(int list_number) {
    int   flag_error = SUCCESS;

    char* list  = load_path_list_file(list_number);
    if(   list == NULL)
	  flag_error = ERROR_MEMORY_ALLOCATION;

    if (flag_error == SUCCESS){
	remove(list);
    	free(list);
    }

    if(flag_error == SUCCESS) {
	FILE *file_info  = fopen(path_to_list_info, "r+");
	if(   file_info == NULL) 
	      flag_error = ERROR_OPEN_FILE;

	if(flag_error == SUCCESS) {
            FILE *temp_file  = fopen("../list_files/temp.txt", "w");
            if(   temp_file == NULL) {
                fclose(file_info);
                flag_error   = ERROR_OPEN_FILE;
	    }
	    if(flag_error == SUCCESS) {
            	char buffer[1024];
            	int  new_line_num = 1;

            	while(fgets(buffer, sizeof(buffer), file_info)) {
                    int  line_num;
            	    char path[256];
            
                    if(sscanf(buffer, "%d. %255[^\n]", &line_num, path) == 2) {
                    	if(line_num != list_number) {
			    fprintf(temp_file, "%d. %s\n", new_line_num, path);
                    	    new_line_num++;
                    	}
                    } else {
                    	fputs(buffer, temp_file);
                    }
            	}

            	fclose(file_info);
            	fclose(temp_file);
	    
        	remove(path_to_list_info);
        	
		rename("../list_files/temp.txt", 
		       path_to_list_info);
	    }
    	}
    }
    
    return flag_error;
}

int delete_node_by_value(LINKED_LIST** list,
			 int	       target) {
    int flag_error  = SUCCESS;
    
    if( list == NULL || *list == NULL)
        flag_error  = ERROR_NULL_POINTER;

    if( flag_error == SUCCESS) {
	NODE* current = (*list)->head;
	NODE* prev = NULL;
    	char  find_flag = 0;
    	
	while( current != NULL && find_flag == 0) {
	    if(current->data == target) {
		if(prev == NULL)
		    (*list)->head = current->next;
		else
		    prev->next    = current->next;
		
		free(current);
		find_flag = 1;
	    } else {
		prev = current;
		current = current->next;
	    }
    	}

	if(find_flag == 0)
	    flag_error = ERROR_TARGET_NOT_FOUND;
    }

    return flag_error;
}

int converting_to_string(LINKED_LIST** list, 
			 char* 	       string, 
			 size_t        size_string) {
    int flag_error = SUCCESS;

    if(list   == NULL || 
      *list   == NULL || 
       string == NULL)
        flag_error = ERROR_NULL_POINTER;
    
    if(size_string == 0)
        flag_error = ERROR_BUFFER_OVERFLOW;
	
    if(flag_error == SUCCESS) {
    	NODE* current     = (*list)->head;
    	char* current_pos = string;
    	size_t remaining  = size_string;

    	string[0] = '\0';
    

    	while(current != NULL) {
	    int written = snprintf(current_pos, remaining, "%d ", current->data);

            if(          written <  0 || 
		 (size_t)written >= remaining)
        	flag_error = ERROR_BUFFER_OVERFLOW; 

            if(flag_error == SUCCESS) {
                current_pos += written;
                remaining   -= written;
                current      = current->next;
            }
        }
    
    
    	if (flag_error == SUCCESS && 
	      current_pos      > string)
	    *(current_pos - 1) = '\0';
    }

    return flag_error;
}

int save_list(LINKED_LIST** list, 
	      char*         path_to_list) {
    int flag_error = SUCCESS;
    if(list         == NULL || 
      *list 	    == NULL || 
       path_to_list == NULL)
	flag_error = ERROR_NULL_POINTER;

    if( flag_error == SUCCESS) {
        size_t result_size = (*list)->size * 20 + 1;
        
	char*  result  = (char*)malloc(result_size);
        if(    result == NULL) 
            flag_error = ERROR_MEMORY_ALLOCATION;
    

    	if(flag_error == SUCCESS)
            flag_error = converting_to_string(list, result, result_size);
	
	if(flag_error == SUCCESS) {
	    save_to_file(result, path_to_list);
	    free(result);
	}
    }

    return flag_error;
}

int print_file_list(int choice_list) {
    int flag_error = SUCCESS;
    
    char* path_to_list  = load_path_list_file(choice_list);
    if(   path_to_list == NULL)
	flag_error = ERROR_MEMORY_ALLOCATION;
    
    if(flag_error == SUCCESS) {
    	FILE* file = fopen(path_to_list, "r");
        if(   file == NULL)
	    flag_error = ERROR_OPEN_FILE;
	
	if(flag_error == SUCCESS) {
	    char buffer[256];
	    if(fgets(buffer, sizeof(buffer), file) != NULL) {
	    	buffer[strcspn(buffer, "\n")] = '\0';
	    	printf("%s",   buffer);
	    }
	    printf("\n");
	    fclose(file);
    	}

    	free(path_to_list);
    }

    return flag_error;
}
