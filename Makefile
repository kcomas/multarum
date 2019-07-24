.POSIX:
.SUFFIXES:

SRC=./src
TEST=./test

CC=gcc
CFLAGS=-std=c11 -g -Wall -Wextra -I $(SRC)

TOKEN_TEST_FILES= $(TEST)/token.o $(SRC)/token.o
token_test: $(TOKEN_TEST_FILES)
	$(CC) $(CFLAGS) -o $@ $(TOKEN_TEST_FILES)

clean:
	rm -fv *_test $(SRC)/*.o $(TEST)/*.o

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<
