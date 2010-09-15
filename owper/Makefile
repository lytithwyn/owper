include ../makeconfig.mk

CFLAGS = -I ./ -g -Wall
OBJS = *.o
SRCS = src/*.cpp
LIBNAME = libowper.a

$(LIBOWPER): $(OBJS) $(SRCS)
	$(CPP) $(CFLAGS) $(SRCS) -c
	ar rcs libowper.a $(OBJS)
	
%.o: src/%.c
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) $(OBJS) $(LIBNAME)