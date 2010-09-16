include makeconfig.mk

CFLAGS = -I ./owper -I ./owper-gui -g -Wall -Werror
GUICFLAGS = `pkg-config --cflags gtk+-2.0`
GUILIBS = `pkg-config --libs gtk+-2.0`
OBJS = *.o
LIBOWPER_DIR = owper
GUISRCS = owper-gui/src/*.cpp
PROG = owpergui

all: $(PROG)

$(LIBOWPER): 
	$(ECHO) "Making all in $(LIBOWPER_DIR)"
	cd $(LIBOWPER_DIR) ; $(MAKE)

owpergui: $(LIBOWPER) $(GUISRCS)
	$(CPP) $(CFLAGS) $(GUICFLAGS) $(GUILIBS) $(GUISRCS) $(LIBOWPER_DIR)/$(LIBOWPER) -o $(PROG)

%.o: owper/src/%.c
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) $(PROG) $(OBJS) $(PROG).dSYM
	$(ECHO) "Making clean in $(LIBOWPER_DIR)"
	cd $(LIBOWPER_DIR) ; $(MAKE) clean
