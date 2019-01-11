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
dicttest.o = $(TEST)/dict.o
file.o = $(SRC)/file.o
filetest.o = $(TEST)/file.o
str.o = $(SRC)/str.o
strtest.o = $(TEST)/str.o
utf8.o = $(SRC)/utf8.o
utf8test.o = $(TEST)/utf8.o
var.o = $(SRC)/var.o
vec.o = $(SRC)/vec.o
vectest.o = $(TEST)/vec.o

tests: file_test dict_test vec_test str_test utf8_test

file_test_deps = $(filetest.o) $(file.o) $(var.o) $(dict.o) $(vec.o) $(str.o) $(utf8.o)
file_test: $(file_test_deps)
	$(CTEST) $(file_test_deps)

dict_test_deps = $(dicttest.o) $(dict.o) $(vec.o) $(var.o) $(file.o) $(str.o) $(utf8.o)
dict_test: $(dict_test_deps)
	$(CTEST) $(dict_test_deps)

vec_test_deps = $(vectest.o) $(vec.o) $(dict.o) $(var.o) $(file.o) $(str.o) $(utf8.o)
vec_test: $(vec_test_deps)
	$(CTEST) $(vec_test_deps)

str_test_deps = $(strtest.o) $(str.o) $(utf8.o)
str_test: $(str_test_deps)
	$(CTEST) $(str_test_deps)

utf8_test_deps = $(utf8test.o) $(utf8.o)
utf8_test: $(utf8_test_deps)
	$(CTEST) $(utf8_test_deps)

.SUFFIXES: .c .o
	$(CC) $(CFLAGS) -c $@ $<

clean:
	rm -f $(SRC)/*.o
	rm -f $(TEST)/*.o
	rm -f *_test
