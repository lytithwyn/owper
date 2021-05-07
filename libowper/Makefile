include ../makeconfig.mk

CFLAGS = -I ./ -g -Wall -Werror
CXXFLAGS = -I ./ -g -Wall
OBJS = $(patsubst %.c,%.o,$(wildcard src/*.c))
CPPOBJS = $(patsubst %.cpp,%.o,$(wildcard src/*.cpp))
SRCS = src/*.cpp

.PHONY: clean

all: $(LIBOWPER)

$(LIBOWPER): $(OBJS) $(SRCS) $(CPPOBJS)
	ar rcs $(LIBOWPER) $(OBJS) $(CPPOBJS)

clean:
	$(RM) $(OBJS) $(LIBOWPER) $(CPPOBJS)
