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

bgcolor.o : bgcolor.cpp bgcolor.hpp global.hpp
	$(CC) $(CFLAGS) -c bgcolor.cpp

eventareas.o : eventareas.cpp eventareas.hpp global.hpp
	$(CC) $(CFLAGS) -c eventareas.cpp

frame.o : frame.cpp frame.hpp global.hpp bgcolor.hpp
	$(CC) $(CFLAGS) -c frame.cpp

element.o : element.cpp element.hpp global.hpp eventareas.hpp frame.hpp
	$(CC) $(CFLAGS) -c element.cpp

taskbar.o : taskbar.cpp taskbar.hpp global.hpp element.hpp
	$(CC) $(CFLAGS) -c taskbar.cpp

window.o : window.cpp window.hpp global.hpp element.hpp
	$(CC) $(CFLAGS) -c window.cpp

textwindow.o : textwindow.cpp textwindow.hpp global.hpp window.hpp
	$(CC) $(CFLAGS) -c textwindow.cpp

listwindow.o : listwindow.cpp listwindow.hpp global.hpp window.hpp
	$(CC) $(CFLAGS) -c listwindow.cpp

clean :
	$(RM) *.o
