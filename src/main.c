#include "lists/lists.h"
#include "output/output.h"

volatile sig_atomic_t keep_running = 1;

int lists_run();

int main() {
    int flag_error = SUCCESS;
    
    signal(SIGINT, handle_signal);

    while(keep_running && flag_error == SUCCESS) {
    	flag_error = lists_run();
	printf("\n");
    }

    return flag_error;
}

int lists_run() {
    int flag_error = SUCCESS;

    printf("Загрузить или создать новый список?:\n"
                              "    1. Загрузить.\n"
                   "    2. Создать новый список.\n");
    int 	 choice_action_list;
    scanf("%d", &choice_action_list);
    printf("\n");

    if(choice_action_list == 1) {
        load_list_info();
        
        printf("Выберите номер списка: ");
	int 	 list_number;
        scanf("%d", &list_number);
	
	printf("Список (%d): ", list_number);
	print_file_list(list_number);
	printf("\n");

	printf("Выберите действие:\n"
	       "1. Добавить число.\n"
	       "2. Удалить список.\n"
	        "3. Удалить число.\n"
			"4. Выйти.\n");
	int 	 choice_action;
	scanf("%d", &choice_action);
	printf("\n");

	if(choice_action == 1) {
	    printf("Введите значение: ");
            int 	 data;
            scanf("%d", &data);
	    printf("\n");
   
            printf("Куда поместить значение?:\n"    
                      	 "1. В начало списка.\n"
                       	  "2. В конец списка.\n"
			     "3. После числа.\n");    
	    int 	 type_insert;
    	    scanf("%d", &type_insert);
	    printf("\n");

	    if(type_insert == 3) {
	    	printf("Введите значение, после которого " 
		       		"необходимо вставить число: ");
	    	int 	     target;
	    	scanf("%d", &target);
		
	    	insert_value_after_node(list_number, target, data);
		
		printf("\n");
	    } else {
    	    	action_main(list_number, data, type_insert);
	    }	
	    
	    printf("Список с новым значением:\n");
	    print_file_list(list_number);
	} else if(choice_action == 2) {
	    delete_list_file(list_number); 
	    printf("Список (%d) был удалён!\n", list_number);
	} else if(choice_action == 3) {
	    printf("Введите удаляемое значение: ");
            int 	 data;
            scanf("%d", &data);
	    printf("\n");

	    action_main(list_number, data, choice_action);

	    printf("Обновлённый список:\n");
	    print_file_list(list_number);
	}
    } else if(choice_action_list == 2) {
	printf("Введите имя файла: ");
	if(create_list_file() != SUCCESS)
    	    printf("Ошибка создания файла!\n");
    }
    
    return flag_error;
}
