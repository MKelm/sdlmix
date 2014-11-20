# Makefile for gui
ARCH := $(shell uname -m)
CC = g++
#CFLAGS = -Wall -ggdb -std=c++0x
CFLAGS = -std=c++0x
LDFLAGS = -lSDL -lSDL_gfx -lSDL_ttf

gui : gui.o
	$(CC) gui.o -o gui.$(ARCH) $(LDFLAGS)

gui.o : gui.cpp
	$(CC) $(CFLAGS) -c gui.cpp

clean :
	$(RM) *.o
