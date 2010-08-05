#!/bin/bash

g++ -I ./ src/main.cpp \
          src/stringManip.cpp \
          src/fileManip.cpp \
          src/hive.cpp \
          src/owpException.cpp \
          -g -Wall -Werror -o owper