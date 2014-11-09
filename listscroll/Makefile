# Makefile for sdl listscroll
ARCH := $(shell uname -m)
CC = gcc
CFLAGS = -Wall -ggdb
LDFLAGS = -lSDL -lSDL_image -lSDL_gfx -lSDL_ttf

listscroll : listscroll.o list.o
	$(CC) listscroll.o list.o -o listscroll.$(ARCH) $(LDFLAGS)

listscroll.o : listscroll.c list.h
	$(CC) $(CFLAGS) -c listscroll.c

list.o : list.c
	$(CC) $(CFLAGS) -c list.c

clean :
	$(RM) *.o
