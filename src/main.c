#include "lists/lists.h"
#include "output/output.h"

int main() {
    int flag_error = SUCCESS;
    
    printf("Загрузить или создать новый список?:\n"
                              "    1. Загрузить.\n"
                   "    2. Создать новый список.\n");
    int choice_action_list;
    scanf("%d", &choice_action_list);
    if(choice_action_list == 1) {
        load_list_info();
        
        printf("Выберите номер списка:\n");
        int list_number;
        scanf("%d", &list_number);
	print_file_list(list_number);

	printf("Выберите действие:\n"
	       "1. Добавить число.\n"
			"2. Выйти.\n");
	int choice_action;
	scanf("%d", &choice_action);
	if(choice_action == 1) {
	    printf("Введите значение:\n");
            int data;
            scanf("%d", &data);
    
            printf("Куда поместить значение?:\n"    
                      "    1. В начало списка.\n"
                       "    2. В конец списка.\n");
	    int type_insert;
    	    scanf("%d", &type_insert);
    	    load_list(list_number, type_insert, data);

	    printf("Список с новым значением:\n");
	    print_file_list(list_number);
	}
    }

    return flag_error;
}
