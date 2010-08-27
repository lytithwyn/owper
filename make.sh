#!/bin/bash

gcc -I ./ -c src/ntreg.c -o ntreg.o

g++ -I ./ src/main.cpp \
          src/stringManip.cpp \
          src/fileManip.cpp \
          src/hive.cpp \
          src/owpException.cpp \
          ntreg.o \
          -g -Wall -o owper
