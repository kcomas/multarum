
CC = gcc
SRC = ./src
CCFLAGS = -std=c99 -g -Wall -Wextra -I $(SRC)

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
