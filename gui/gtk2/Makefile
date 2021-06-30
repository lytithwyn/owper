include ../../makeconfig.mk
include makeconfig.mk

CPPOBJS = $(patsubst %.cpp,%.o,$(wildcard src/*.cpp))
SRCS = src/*.cpp
CXXFLAGS = -I ./  -I ../../$(LIBOWPER_DIR)/ -g -Wno-deprecated-declarations $(GTK2LIBS) $(GTK2CFLAGS)

.PHONY: clean

all: $(GTK2GUIA)

$(GTK2GUIA): $(SRCS) $(CPPOBJS)
	ar rcs $(GTK2GUIA) $(CPPOBJS)

clean:
	$(RM) $(GTK2GUIA) $(CPPOBJS)
