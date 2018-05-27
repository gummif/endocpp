
TARGET = test
OUTDIR = /usr/bin
INCDIR = ../include
CC = gcc -std=c99
CXX = g++ -std=c++17
RM = rm -f
INSTALL = install

# default is release mode
debug ?= 0
ifeq ($(debug), 0)
	CFLAGS += -O2 -DNDEBUG
else
	CFLAGS += -Og -g -DDEBUG
	LDFLAGS += -g
endif
