include ../../makeconfig.mk

CPPOBJS = $(patsubst %.cpp,%.o,$(wildcard src/*.cpp))
SRCS = src/*.cpp
GTK2CFLAGS = `pkg-config --cflags gtk+-2.0`
CXXFLAGS = -I ./  -I ../../$(LIBOWPER_DIR)/ -g -Wall $(GTK2LIBS) $(GTK2CFLAGS)

.PHONY: clean

all: $(GTK2GUIA)

$(GTK2GUIA): $(SRCS) $(CPPOBJS)
	ar rcs $(GTK2GUIA) $(CPPOBJS)

clean:
	$(RM) $(GTK2GUIA) $(CPPOBJS)
