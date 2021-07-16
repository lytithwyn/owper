AUTOMAKE_OPTIONS = subdir-objects
bin_PROGRAMS = owper_gtk
owper_gtk_SOURCES = src/main_gtk.cpp  src/owperGUI.cpp src/userWidget.cpp
AM_CPPFLAGS = -I$(top_srcdir)/libowper $(gtk2_CFLAGS)
AM_LDFLAGS = $(top_srcdir)/libowper/libowper.a $(gtk2_LIBS) $(OpenSSL_LIBS)
