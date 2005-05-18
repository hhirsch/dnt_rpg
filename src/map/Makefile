CC = gcc
CPP = g++
CFLAGS = -g -Wall -c -DDEBUG_MAP
CLIBSDIR = -L/usr/X11/lib
CLIBS = -lGLU -lGL -lSDL -lSDL_image

all: teste

map.o: map.cpp map.h
	$(CPP) $(CFLAGS) map.cpp -o map.o

teste: map.o
	$(CPP) $(CFLAGS) teste.c -o teste.o
	$(CPP) $(CLIBSDIR) $(CLIBS) -g -Wall teste.o map.o -o teste

run: teste
	./teste

clean: 
	\rm -Rf ./*.o ./teste 
