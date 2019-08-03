.POSIX:
.SUFFIXES:

SRC=./src
TEST=./test

CC=gcc
CFLAGS=-std=c11 -g -Wall -Wextra -I $(SRC)

PARSER_TEST_FILES=$(TEST)/parser.o $(SRC)/parser.o $(SRC)/token.o
parser_test: $(PARSER_TEST_FILES)
	$(CC) $(CFLAGS) -o $@ $(PARSER_TEST_FILES)

TOKEN_TEST_FILES=$(TEST)/token.o $(SRC)/token.o
token_test: $(TOKEN_TEST_FILES)
	$(CC) $(CFLAGS) -o $@ $(TOKEN_TEST_FILES)

clean:
	rm -fv *_test $(SRC)/*.o $(TEST)/*.o

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<
