include ../../makeconfig.mk

CPPOBJS = $(patsubst %.cpp,%.o,$(wildcard src/*.cpp))
SRCS = src/*.cpp
CXXFLAGS = -I ./  -I ../../$(LIBOWPER_DIR)/ -g -Wno-deprecated-declarations `pkg-config --cflags gtk+-2.0`
LDFLAGS = ../../$(LIBOWPER_DIR)/$(LIBOWPER) $(LIBOWPER_LDFLAGS) `pkg-config --libs gtk+-2.0`

.PHONY: clean

all: $(PROG)_gtk

$(PROG)_gtk: $(CPPOBJS) $(LIBOWPER)
	$(CXX) $(CPPOBJS) $(LDFLAGS) -o ../../$(PROG)_gtk

$(LIBOWPER):
	$(ECHO) "Making all in $(LIBOWPER_DIR)"
	cd ../../$(LIBOWPER_DIR) ; $(MAKE)

clean:
	$(RM) $(GTK2GUIA) $(CPPOBJS)
