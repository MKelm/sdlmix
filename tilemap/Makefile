# Makefile for sdl tilemap
ARCH := $(shell uname -m)
CC = gcc
CFLAGS = -Wall -ggdb
LDFLAGS = -lSDL -lSDL_image -lSDL_gfx -lSDL_ttf

tilemap : tilemap.o map.o tiles.o
	$(CC) tilemap.o map.o tiles.o -o tilemap.$(ARCH) $(LDFLAGS)

tilemap.o : tilemap.c map.h
	$(CC) $(CFLAGS) -c tilemap.c

map.o : map.c map.h tiles.h
	$(CC) $(CFLAGS) -c map.c

tiles.o : tiles.c tiles.h
	$(CC) $(CFLAGS) -c tiles.c

clean :
	$(RM) *.o
