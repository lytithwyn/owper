CC = gcc
CPP = g++
CFLAGS = -I ./owper -I ./owper-gui -g -Wall
GUICFLAGS = `pkg-config --cflags gtk+-2.0`
GUILIBS = `pkg-config --libs gtk+-2.0`
RM = rm -rf
OBJS = *.o
LIBSRCS = owper/src/*.cpp
GUISRCS = owper-gui/src/*.cpp
PROG = owpergui

all: $(PROG)

owper: $(OBJS) $(LIBSRCS)
	$(CPP) $(CFLAGS) $(LIBSRCS) -c

owpergui: $(OBJS) $(GUISRCS)
	$(CPP) $(CFLAGS) $(GUICFLAGS) $(GUILIBS) $(GUISRCS) $(OBJS) -o $(PROG)

%.o: owper/src/%.c
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) $(PROG) $(OBJS) $(PROG).dSYM
