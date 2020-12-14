CC=gcc
ERRFLAGS=-Werror -Wall -Wextra -Wstrict-prototypes
CFLAGS=-std=c11 -O2 $(ERRFLAGS)
BINARY_NAME=advent_2020_c

OBJS = \
	main.o \
	bitvector.o \
	day_one.o \
	day_two.o \
	day_three.o \
	day_four.o \
	day_five.o

HEADERS = \
	bitvector.h \
	macros.h

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BINARY_NAME): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)


.PHONY: clean
clean:
	find . -name '*.[oa]' -exec rm -f {} ';'
	rm -f $(BINARY_NAME)