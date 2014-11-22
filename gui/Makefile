# Makefile for gui
ARCH := $(shell uname -m)
CC = g++
#CFLAGS = -Wall -ggdb -std=c++0x
#CFLAGS = -std=c++0x -O3
CFLAGS = -std=c++0x
LDFLAGS = -lSDL -lSDL_ttf -lSDL_image -lSDL_gfx
OBJECTS = gui.o bgcolor.o eventareas.o frame.o element.o taskbar.o window.o \
textwindow.o listwindow.o
HEADERS = global.hpp bgcolor.hpp eventareas.hpp frame.hpp element.hpp \
taskbar.hpp window.hpp textwindow.hpp listwindow.hpp

gui : $(OBJECTS)
	$(CC) $(OBJECTS) -o gui.$(ARCH) $(LDFLAGS)

gui.o : gui.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c gui.cpp

clean :
	$(RM) *.o
