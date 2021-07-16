AUTOMAKE_OPTIONS = subdir-objects
SUBDIRS = libowper gui/gtk2
ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = owper
owper_SOURCES = main.cpp
owper_DEPENDENCIES = libowper
owper_CPPFLAGS = -I ./libowper
owper_LDFLAGS = ./libowper/libowper.a $(OpenSSL_LIBS)
