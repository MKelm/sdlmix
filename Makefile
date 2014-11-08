# Makefile for nceventlist
ARCH := $(shell uname -m)
CC = gcc
CFLAGS = -Wall -ggdb
LDFLAGS = -lSDL -lSDL_image -lSDL_gfx -lSDL_ttf

starfield : starfield.o
	$(CC) starfield.o -o starfield.$(ARCH) $(LDFLAGS)

starfield.o : starfield.c
	$(CC) $(CFLAGS) -c starfield.c

clean :
	$(RM) *.o
