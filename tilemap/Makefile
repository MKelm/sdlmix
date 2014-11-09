# Makefile for sdl tilemap
ARCH := $(shell uname -m)
CC = gcc
CFLAGS = -Wall -ggdb
LDFLAGS = -lSDL -lSDL_image -lSDL_gfx -lSDL_ttf

tilemap : tilemap.o tiles.o
	$(CC) tilemap.o tiles.o -o tilemap.$(ARCH) $(LDFLAGS)

tilemap.o : tilemap.c tiles.h
	$(CC) $(CFLAGS) -c tilemap.c

tiles.o : tiles.c tiles.h
	$(CC) $(CFLAGS) -c tiles.c

clean :
	$(RM) *.o
