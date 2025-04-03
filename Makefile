CC = gcc

CFLAGS = -Wall -Wextra -g -std=c11 -DDEBUG -I$(SRC_DIR) -I$(LISTS_DIR) -I$(OUTPUT_DIR) -fsanitize=address

SRC_DIR = src
LISTS_DIR = $(SRC_DIR)/lists
OUTPUT_DIR = $(SRC_DIR)/output

OBJ = $(SRC_DIR)/main.o $(LISTS_DIR)/lists.o $(OUTPUT_DIR)/output.o

BIN_DIR = build
TARGET = $(BIN_DIR)/lists_main

SRCS = $(SRC_DIR)/main.c $(LISTS_DIR)/lists.c $(OUTPUT_DIR)/output.c

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

$(SRC_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@
$(LISTS_DIR)%.o: $(LISTS_DIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@
$(OUTPUT_DIR)%.o: $(OUTPUT_DIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

rebuild: clean $(TARGET)
