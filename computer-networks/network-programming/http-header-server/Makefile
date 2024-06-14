CC = clang
CFLAGS = -g -O0
AS = clang
ASFLAGS = -g -O0
TARGET = a.out
SRC_C = main.c
SRC_S = hhtojson.s
OBJS = main.o hhtojson.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
