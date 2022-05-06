AUTOMAKE_OPTIONS = subdir-objects
SUBDIRS = libowper gui/base

if HAVE_GTK2
SUBDIRS += gui/gtk2
endif

if HAVE_GTK3
SUBDIRS += gui/gtk3
endif

bin_PROGRAMS = owper
owper_SOURCES = main.cpp
owper_DEPENDENCIES = libowper
owper_CPPFLAGS = -I ./libowper
owper_LDFLAGS = ./libowper/libowper.a $(OpenSSL_LIBS)
