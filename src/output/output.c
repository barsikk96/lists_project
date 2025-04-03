#include "output.h"

const char *result_file = {"../results/result.txt"};

void erase_file_data(const char *path) {
  FILE *result_file = fopen(path, "w");
  fclose(result_file);
}

void save_to_file(const char *data, const char *path) {
  FILE *file = fopen(path, "w");
  if (file) {
    fprintf(file, "%s", data);
    fclose(file);
  }
}

void print_data(const char *data, int type_of_output) {
  if (data) {
    switch (type_of_output) {
    case 1:
      puts(data);
      break;
    case 2:
      save_to_file(data, result_file);
      printf("Данные были записаны в файл result.txt!\n");
      break;
    default:
      printf("Вы ввели неверное число! Попробуйте ещё раз!\n");
      break;
    }
  } else {
    printf("Ошибка: нет данных для вывода!\n");
  }
}
