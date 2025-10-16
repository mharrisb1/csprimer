CC := cc
CFLAGS := -Wall -Wextra -std=c11 -g -O0 -Iinclude
DEBUG ?= 0

ifeq ($(DEBUG),1)
	CFLAGS += -DDEBUG
endif

SRC_DIR := src
BUILD_DIR := build

SRCS := $(shell find $(SRC_DIR) -type f -name '*.c')
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)

TARGET := $(BUILD_DIR)/shell.o

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
