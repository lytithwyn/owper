include makeconfig.mk

LIBOWPER_DIR = libowper
CFLAGS = -I ./$(LIBOWPER_DIR) -I ./owper-gui -g -Wall -Werror -Wno-deprecated-declarations -Wno-unused-but-set-variable `pkg-config --cflags openssl`
LIBS = `pkg-config --libs openssl`
GUICFLAGS = `pkg-config --cflags gtk+-2.0`
GUILIBS = `pkg-config --libs gtk+-2.0`
OBJS = *.o
GUISRCS = owper-gui/src/*.cpp
PROG = owpergui

.PHONY: all clean

all: $(PROG)

$(LIBOWPER):
	$(ECHO) "Making all in $(LIBOWPER_DIR)"
	cd $(LIBOWPER_DIR) ; $(MAKE)

owpergui: $(LIBOWPER) $(GUISRCS)
	$(CPP) $(GUISRCS) $(CFLAGS) $(GUICFLAGS) $(GUILIBS) $(LIBOWPER_DIR)/$(LIBOWPER) $(LIBS) -o $(PROG)

clean:
	$(RM) $(PROG) $(OBJS) $(PROG).dSYM
	$(ECHO) "Making clean in $(LIBOWPER_DIR)"
	cd $(LIBOWPER_DIR) ; $(MAKE) clean
