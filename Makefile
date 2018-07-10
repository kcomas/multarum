
CC = gcc
SRC = ./src
OP ?= -g
CCFLAGS = -std=c99 $(OP) -Wall -Wextra -I $(SRC)

SOURCES = $(wildcard $(SRC)/*.c)
OBJECTS = $(SOURCES:.c=.o)

all: multarum

%.o: %.c
	$(CC) -c -o $@ $< $(CCFLAGS)

multarum: $(OBJECTS)
	$(CC) -o $@ $^

.PHONY: clean
clean:
	rm -rf multarum
	rm -rf $(SRC)/*.o
