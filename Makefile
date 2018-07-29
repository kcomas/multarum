
CC = gcc
SRC = ./src
OP = -O3
CCFLAGS = -std=c99 $(OP) -Wall -Wextra -I $(SRC)

SOURCES = $(wildcard $(SRC)/*.c)
OBJECTS = $(SOURCES:.c=.o)

all: multarum

debug: OP = -g
debug: multarum

%.o: %.c
	$(CC) -c -o $@ $< $(CCFLAGS)

multarum: $(OBJECTS)
	$(CC) -o $@ $^

.PHONY: clean
clean:
	rm -rf multarum
	rm -rf $(SRC)/*.o
