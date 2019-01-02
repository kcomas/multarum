.POSIX:
.SUFFIXES:
CC = gcc
INC = include
SRC = src
TEST = test
OP = -g
CFLAGS = -std=c99 $(OP) -Wall -Wextra -I $(INC)

dict.o = $(SRC)/dict.o
file.o = $(SRC)/file.o
str.o = $(SRC)/str.o
utf8.o = $(SRC)/utf8.o
utf8test.o = $(TEST)/utf8.o
var.o = $(SRC)/var.o
vec.o = $(SRC)/vec.o

utf8_test: $(utf8test.o) $(utf8.o) $(INC)/test.h
	$(CC) $(CFLAGS) -o $@  $(utf8test.o) $(utf8.o)

$(dict.o): $(SRC)/dict.c $(INC)/dict.h
$(file.o): $(SRC)/file.c $(INC)/file.h
$(str.o): $(SRC)/str.c $(INC)/str.h
$(utf8.o): $(SRC)/utf8.c $(INC)/utf8.h
$(var.o): $(SRC)/var.c $(INC)/var.h
$(vec.o): $(SRC)/vec.c $(INC)/vec.h

.SUFFIXES: .c .o
	$(CC) $(CFLAGS) -c $@ $<

clean:
	rm -f $(SRC)/*.o
	rm -f $(TEST)/*.o
	rm -f *_test
