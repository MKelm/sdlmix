# Makefile for cpp test
ARCH := $(shell uname -m)
CC = g++
CFLAGS = -Wall -ggdb
LDFLAGS = -lSDL -lSDL_image -lSDL_gfx -lSDL_ttf

cpptest : cpptest.o
	$(CC) cpptest.o -o cpptest.$(ARCH) $(LDFLAGS)

cpptest.o : cpptest.cpp
	$(CC) $(CFLAGS) -c cpptest.cpp

clean :
	$(RM) *.o
