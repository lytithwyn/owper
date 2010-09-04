CC = gcc
CPP = g++
CFLAGS = -I ./ -g -Wall
RM = rm -rf
OBJS = ntreg.o
SRCS = src/*.cpp
PROG = owper

all: $(PROG)

owper: $(OBJS) $(SRCS)
	$(CPP) $(CFLAGS) $(SRCS) $(OBJS) -o $(PROG)

ntreg.o:
	$(CC) $(CFLAGS) -c src/ntreg.c

clean:
	$(RM) $(PROG) $(OBJS) $(PROG).dSYM