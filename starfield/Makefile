# Makefile for sdl starfield
ARCH := $(shell uname -m)
CC = gcc
CFLAGS = -Wall -ggdb
LDFLAGS = -lSDL -lSDL_image -lSDL_gfx -lSDL_ttf

starfield : starfield.o stars.o
	$(CC) starfield.o stars.o -o starfield.$(ARCH) $(LDFLAGS)

starfield.o : starfield.c stars.h
	$(CC) $(CFLAGS) -c starfield.c

stars.o : stars.c stars.h
	$(CC) $(CFLAGS) -c stars.c

clean :
	$(RM) *.o
