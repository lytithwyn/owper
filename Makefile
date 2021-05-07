include makeconfig.mk

LIBOWPER_DIR = libowper
LIBS = `pkg-config --libs openssl`
OBJS = *.o
GUIROOT = owper-gui
GTK2GUIDIR = $(GUIROOT)/gtk2
GTK2CFLAGS = `pkg-config --cflags gtk+-2.0`
GTK2LIBS = `pkg-config --libs gtk+-2.0`
GTK2SRCS = $(GTK2GUIDIR)/src/*.cpp
CFLAGS = -I ./$(LIBOWPER_DIR) -I ./$(GTK2GUIDIR) -g -Wall -Werror -Wno-deprecated-declarations -Wno-unused-but-set-variable `pkg-config --cflags openssl`
PROG = owper

.PHONY: all clean

all: $(PROG)

$(LIBOWPER):
	$(ECHO) "Making all in $(LIBOWPER_DIR)"
	cd $(LIBOWPER_DIR) ; $(MAKE)

$(PROG): $(LIBOWPER) $(GTK2SRCS)
	$(CPP) $(GTK2SRCS) $(CFLAGS) $(GTK2CFLAGS) $(GTK2LIBS) $(LIBOWPER_DIR)/$(LIBOWPER) $(LIBS) -o $(PROG)

clean:
	$(RM) $(PROG) $(OBJS) $(PROG).dSYM
	$(ECHO) "Making clean in $(LIBOWPER_DIR)"
	cd $(LIBOWPER_DIR) ; $(MAKE) clean
