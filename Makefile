.POSIX:
.SUFFIXES:
CC = gcc
INC = ./include
SRC = ./src
TEST = ./test
OP = -g
CCFLAGS = -std=c99 $(OP) -I $(SRC)
