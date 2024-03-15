AS = nasm

CFLAGS = -g -Wall -arch x86_64

ifeq ($(shell uname -s),Darwin)
	ASFLAGS = -g -f macho64 --prefix _
else
	ASFLAGS = -g -f elf64
endif

C_OBJS = $(patsubst %.c,%.o,$(wildcard *.c))
AS_OBJS = $(patsubst %.asm,%.o,$(wildcard *.asm))

all: tests
	./$<

tests: $(C_OBJS) $(AS_OBJS)
	$(CC) $(CFLAGS) -o $@ $(C_OBJS) $(AS_OBJS)

%.o: %.asm
	$(AS) $(ASFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.o tests

.PHONY: all clean
