include makeconfig.mk

LDFLAGS = $(LIBOWPER_LDFLAGS)
OBJS = *.o
CXXFLAGS = -I ./$(LIBOWPER_DIR)

.PHONY: all clean

all: $(PROG) gtk2 ncurses

$(LIBOWPER):
	$(ECHO) "Making all in $(LIBOWPER_DIR)"
	cd $(LIBOWPER_DIR) ; $(MAKE)

$(PROG): $(LIBOWPER)
	$(CPP) main.cpp $(CXXFLAGS) $(LIBOWPER_DIR)/$(LIBOWPER) $(LDFLAGS) -o $(PROG)

gtk2: $(LIBOWPER)
	$(ECHO) "Making all in $(GUIROOT)/$(GTK2GUIDIR)"
	cd $(GUIROOT)/$(GTK2GUIDIR) ; $(MAKE)

ncurses: $(LIBOWPER) main_ncurses.o
	$(CPP) main_ncurses.o $(CFLAGS) $(LIBOWPER_DIR)/$(LIBOWPER) $(LDFLAGS) -o $(PROG)_ncurses

clean:
	$(RM) $(PROG) $(PROG)_gtk $(OBJS) $(PROG).dSYM
	$(ECHO) "Making clean in $(LIBOWPER_DIR)"
	cd $(LIBOWPER_DIR) ; $(MAKE) clean
	$(ECHO) "Making clean in $(GUIROOT)/$(GTK2GUIDIR)"
	cd $(GUIROOT)/$(GTK2GUIDIR) ; $(MAKE) clean
