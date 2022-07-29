AUTOMAKE_OPTIONS = subdir-objects
bin_PROGRAMS = owper_ncurses
owper_ncurses_SOURCES = src/main_ncurses.cpp  src/owper_ncurses.cpp src/userWidget.cpp
AM_CPPFLAGS = -I$(top_srcdir)/libowper -I$(top_srcdir)/gui/base $(ncurses_CFLAGS)
AM_LDFLAGS = $(top_srcdir)/gui/base/libbaseowpergui.a $(top_srcdir)/libowper/libowper.a $(ncurses_LIBS) $(OpenSSL_LIBS)
