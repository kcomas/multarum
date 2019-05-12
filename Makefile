.POSIX:
.SUFFIXES:

SRC=src
INC=src

CC=gcc
CFLAGS=-std=c99 -g -Wall -Wextra -I $(INC)

OBJECTS=$(SRC)/main.o $(SRC)/str.o $(SRC)/file.o

all: multarum

multarum: $(OBJECTS)
	$(CC) $(CCFLAGS) -o multarum $(OBJECTS)

.SUFFIXES: .c .o
	$(CC) $(CFLAGS) -c $@ $<

clean:
	rm -f $(SRC)/*.o
	rm -f multarum
