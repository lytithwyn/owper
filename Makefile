CC = gcc
CPP = g++
CFLAGS = -I ./ -g -Wall
RM = rm -rf
OBJS = *.o
SRCS = src/*.cpp
PROG = owper

all: $(PROG)

owper: $(OBJS) $(SRCS)
	$(CPP) $(CFLAGS) $(SRCS) $(OBJS) -o $(PROG)

%.o: src/%.c
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) $(PROG) $(OBJS) $(PROG).dSYM
