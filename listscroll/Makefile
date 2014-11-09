# Makefile for sdl listscroll
ARCH := $(shell uname -m)
CC = gcc
CFLAGS = -Wall -ggdb
LDFLAGS = -lSDL -lSDL_image -lSDL_gfx -lSDL_ttf

listscroll : listscroll.o list.o loader.o ../jsmn/jsmn.o
	$(CC) listscroll.o list.o loader.o ../jsmn/jsmn.o -o listscroll.$(ARCH) $(LDFLAGS)

listscroll.o : listscroll.c global.h list.h
	$(CC) $(CFLAGS) -c listscroll.c

list.o : list.c global.h list.h loader.h
	$(CC) $(CFLAGS) -c list.c

loader.o : loader.c global.h loader.h list.h ../jsmn/jsmn.h
	$(CC) $(CFLAGS) -c loader.c

clean :
	$(RM) *.o
