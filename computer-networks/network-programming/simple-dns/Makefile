# Define the compiler
CC = clang

# Define the directories
INCDIR = include
SRCDIR = src
BUILDDIR = build

# Define the source files and object files
SRCS = $(wildcard $(SRCDIR)/*.c) main.c
OBJS = $(SRCS:.c=.o)

# Define the output executable
TARGET = main

# Compiler flags
CFLAGS = -I$(INCDIR) -g -O0

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

# Rule to build the object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)
