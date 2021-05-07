include makeconfig.mk

LIBS = `pkg-config --libs openssl`
OBJS = *.o
GUIROOT = gui
GTK2GUIDIR = $(GUIROOT)/gtk2
GTK2LIBS = `pkg-config --libs gtk+-2.0`
CFLAGS = -I ./$(LIBOWPER_DIR) -I -g `pkg-config --cflags openssl`
PROG = owper

.PHONY: all clean

all: $(PROG)

$(LIBOWPER):
	$(ECHO) "Making all in $(LIBOWPER_DIR)"
	cd $(LIBOWPER_DIR) ; $(MAKE)

$(GTK2GUIA):
	$(ECHO) "Making all in $(GTK2GUIDIR)"
	cd $(GTK2GUIDIR) ; $(MAKE)

$(PROG): $(LIBOWPER) $(GTK2GUIA)
	$(CPP) $(GTK2GUIDIR)/$(GTK2GUIA) $(CFLAGS) $(LIBOWPER_DIR)/$(LIBOWPER) $(GTK2LIBS) $(LIBS) -o $(PROG)

clean:
	$(RM) $(PROG) $(OBJS) $(PROG).dSYM
	$(ECHO) "Making clean in $(LIBOWPER_DIR)"
	cd $(LIBOWPER_DIR) ; $(MAKE) clean
	$(ECHO) "Making clean in $(GTK2GUIDIR)"
	cd $(GTK2GUIDIR) ; $(MAKE) clean
