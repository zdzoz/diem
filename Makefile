# Copyright: Oskar Zdziarski 2023

CFLAGS = -Wall -Wextra -Werror -pedantic -std=c11
LDFLAGS = -fsanitize=address,undefined

DEBUG = -g3 -O0

BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin
SRC_DIR = src

default: all

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<"
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(DEBUG) -c -o $@ $<

TARGET = $(BIN_DIR)/diem
$(TARGET): $(OBJS)
	@echo "Linking $@"
	@mkdir -p $(BIN_DIR)
	@$(CC) $(CFLAGS) $(DEBUG) -o $@ $^ $(LDFLAGS)

.PHONY: all clean default

all: $(TARGET)

run: $(TARGET)
	@$(TARGET)

clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR)

