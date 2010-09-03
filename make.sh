#!/bin/bash

gcc -I ./ -g -c src/ntreg.c -o ntreg.o

g++ -I ./ src/main.cpp \
          src/stringManip.cpp \
          src/fileManip.cpp \
          src/hive.cpp \
          src/owpException.cpp \
          src/samHive.cpp \
          ntreg.o \
          -g -Wall -o owper
