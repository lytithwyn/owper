AUTOMAKE_OPTIONS = subdir-objects
SUBDIRS = libowper gui/base gui/gtk2

bin_PROGRAMS = owper
owper_SOURCES = main.cpp
owper_DEPENDENCIES = libowper
owper_CPPFLAGS = -I ./libowper
owper_LDFLAGS = ./libowper/libowper.a $(OpenSSL_LIBS)
