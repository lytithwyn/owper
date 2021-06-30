include makeconfig.mk

LIBS = `pkg-config --libs openssl`
OBJS = *.o
CFLAGS = -I ./$(LIBOWPER_DIR)  -g `pkg-config --cflags openssl`
CXXFLAGS = -I ./$(LIBOWPER_DIR)
PROG = owper

include $(GUIROOT)/$(GTK2GUIDIR)/makeconfig.mk

.PHONY: all clean

all: $(PROG) gtk2 ncurses

$(LIBOWPER):
	$(ECHO) "Making all in $(LIBOWPER_DIR)"
	cd $(LIBOWPER_DIR) ; $(MAKE)

$(GTK2GUIA):
	$(ECHO) "Making all in $(GUIROOT)/$(GTK2GUIDIR)"
	cd $(GUIROOT)/$(GTK2GUIDIR) ; $(MAKE)

$(PROG): $(LIBOWPER) main.o
	$(CPP) main.o $(CFLAGS) $(LIBOWPER_DIR)/$(LIBOWPER) $(LIBS) -o $(PROG)


gtk2: $(LIBOWPER) $(GTK2GUIA)
	$(CPP) main_gtk.cpp $(GUIROOT)/$(GTK2GUIDIR)/$(GTK2GUIA) $(CFLAGS) -I ./$(GUIROOT)/$(GTK2GUIDIR) $(GTK2CFLAGS) $(LIBOWPER_DIR)/$(LIBOWPER) $(GTK2LIBS) $(LIBS) -o $(PROG)_gtk

ncurses: $(LIBOWPER) main_ncurses.o
	$(CPP) main_ncurses.o $(CFLAGS) $(LIBOWPER_DIR)/$(LIBOWPER) $(LIBS) -o $(PROG)_ncurses

clean:
	$(RM) $(PROG) $(OBJS) $(PROG).dSYM
	$(ECHO) "Making clean in $(LIBOWPER_DIR)"
	cd $(LIBOWPER_DIR) ; $(MAKE) clean
	$(ECHO) "Making clean in $(GUIROOT)/$(GTK2GUIDIR)"
	cd $(GUIROOT)/$(GTK2GUIDIR) ; $(MAKE) clean
