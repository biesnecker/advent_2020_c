CC=gcc
ERRFLAGS=-Werror -Wall -Wextra -Wstrict-prototypes
CFLAGS=-std=c17 -O0 -g -march=native $(ERRFLAGS)
BINARY_NAME=advent_2020_c

OBJS = \
	main.o \
	bitvector.o \
	fnv.o \
	day_one.o \
	day_two.o \
	day_three.o \
	day_four.o \
	day_five.o \
	day_six.o \
	day_seven.o \
	day_eight.o \
	day_nine.o \
	day_ten.o \
	day_eleven.o \
	day_twelve.o \
	day_thirteen.o \
	day_fourteen.o \
	day_fifteen.o \
	day_sixteen.o \
	day_seventeen.o \
	day_eighteen.o \
	day_twentytwo.o

HEADERS = \
	bitvector.h \
	fnv.h \
	macros.h

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BINARY_NAME): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)


.PHONY: clean
clean:
	find . -name '*.[oa]' -exec rm -f {} ';'
	rm -f $(BINARY_NAME)