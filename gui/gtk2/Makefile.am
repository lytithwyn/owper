AUTOMAKE_OPTIONS = subdir-objects
bin_PROGRAMS = owper_gtk
owper_gtk_SOURCES = src/main_gtk.cpp  src/owperGUI.cpp src/userWidget.cpp
AM_CPPFLAGS = -I$(top_srcdir)/libowper -I$(top_srcdir)/gui/base $(gtk2_CFLAGS) -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED
AM_LDFLAGS = $(top_srcdir)/gui/base/libbaseowpergui.a $(top_srcdir)/libowper/libowper.a $(gtk2_LIBS) $(OpenSSL_LIBS)
