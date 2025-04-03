#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>

void erase_file_data(const char *path);

void save_to_file(const char *data, const char *path);

void print_data(const char *data, int type_of_output);

#endif
