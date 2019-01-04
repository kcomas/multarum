.POSIX:
.SUFFIXES:
CC = gcc
INC = include
SRC = src
TEST = test
OP = -g
CFLAGS = -std=c99 $(OP) -Wall -Wextra -I $(INC)
CTEST = $(CC) $(CFLAGS) -o $@

dict.o = $(SRC)/dict.o
file.o = $(SRC)/file.o
str.o = $(SRC)/str.o
strtest.o = $(TEST)/str.o
utf8.o = $(SRC)/utf8.o
utf8test.o = $(TEST)/utf8.o
var.o = $(SRC)/var.o
vec.o = $(SRC)/vec.o
vectest.o = $(TEST)/vec.o

tests: vec_test str_test utf8_test

vec_test: $(vectest.o) $(vec.o) $(var.o) $(str.o) $(utf8.o)
	$(CTEST) $(vectest.o) $(vec.o) $(var.o) $(str.o) $(utf8.o)

str_test: $(strtest.o) $(str.o) $(utf8.o)
	$(CTEST) $(strtest.o) $(str.o) $(utf8.o)

utf8_test: $(utf8test.o) $(utf8.o)
	$(CTEST) $(utf8test.o) $(utf8.o)

.SUFFIXES: .c .o
	$(CC) $(CFLAGS) -c $@ $<

clean:
	rm -f $(SRC)/*.o
	rm -f $(TEST)/*.o
	rm -f *_test
