#Makefile for Assignment 5

CC = gcc209
CCFLAGS = -g  -D_GNU_SOURCE

all: ish

ish: execution.o dfa.o syntactic.o dynarray.o
	$(CC) $(CCFLAGS) execution.o dfa.o syntactic.o dynarray.o -o ish
execution.o: execution.c syntactic.c dfa.c dfa.h dynarray.h
	$(CC) $(CCFLAGS) -c execution.c 
syntactic.o: syntactic.c dfa.c dfa.h dynarray.h
	$(CC) $(CCFLAGS) -c syntactic.c
dfa.o: dfa.c dfa.h dynarray.h
	$(CC) $(CCFLAGS) -c dfa.c
dynarray.o: dynarray.c
	$(CC) $(CCFLAGS) -c dynarray.c

clean:
	rm -f execution.o syntactic.o dfa.o dynarray.o ish