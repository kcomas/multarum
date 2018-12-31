.POSIX:
.SUFFIXES:
CC = gcc
INC = ./include
SRC = ./src
TEST = ./test
OP = -O3
CCFLAGS = -std=c99 $(OP) -I $(INC)
