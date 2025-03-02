CC = gcc
CFLAGS += -Werror -Wall -Wpedantic


all: CFLAGS += -O2
all: arith
	rm -f $<.o

asan: CFLAGS += -g -fsanitize=address
asan: arith.o
	$(CC) $(CFLAGS) $< -o $@.out
	rm -f $<

lsan: CFLAGS += -g -fsanitize=leak
lsan: arith.o
	$(CC) $(CFLAGS) $< -o $@.out
	rm -f $<

ubsan: CFLAGS += -g -fsanitize=undefined
ubsan: arith.o
	$(CC) $(CFLAGS) $< -o $@.out
	rm -f $<

clean: 
	rm -f arith *.out *.o

.PHONY: all clean


