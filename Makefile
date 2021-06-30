include makeconfig.mk

LIBS = `pkg-config --libs openssl`
OBJS = *.o
GUIROOT = gui
GTK2GUIDIR = $(GUIROOT)/gtk2
GTK2LIBS = `pkg-config --libs gtk+-2.0`
CFLAGS = -I ./$(LIBOWPER_DIR)  -g `pkg-config --cflags openssl`
CXXFLAGS = -I ./$(LIBOWPER_DIR) -I ./$(GTK2GUIDIR) -g `pkg-config --cflags gtk+-2.0`
PROG = owper

.PHONY: all clean

all: $(PROG) gtk2 ncurses

$(LIBOWPER):
	$(ECHO) "Making all in $(LIBOWPER_DIR)"
	cd $(LIBOWPER_DIR) ; $(MAKE)

$(GTK2GUIA):
	$(ECHO) "Making all in $(GTK2GUIDIR)"
	cd $(GTK2GUIDIR) ; $(MAKE)

$(PROG): $(LIBOWPER) main.o
	$(CPP) main.o $(CFLAGS) $(LIBOWPER_DIR)/$(LIBOWPER) $(LIBS) -o $(PROG)


gtk2: $(LIBOWPER) $(GTK2GUIA) main_gtk.o
	$(CPP) main_gtk.o $(GTK2GUIDIR)/$(GTK2GUIA) $(CFLAGS) $(LIBOWPER_DIR)/$(LIBOWPER) $(GTK2LIBS) $(LIBS) -o $(PROG)_gtk

ncurses: $(LIBOWPER) main_ncurses.o
	$(CPP) main_ncurses.o $(CFLAGS) $(LIBOWPER_DIR)/$(LIBOWPER) $(LIBS) -o $(PROG)_ncurses

clean:
	$(RM) $(PROG) $(OBJS) $(PROG).dSYM
	$(ECHO) "Making clean in $(LIBOWPER_DIR)"
	cd $(LIBOWPER_DIR) ; $(MAKE) clean
	$(ECHO) "Making clean in $(GTK2GUIDIR)"
	cd $(GTK2GUIDIR) ; $(MAKE) clean
