# DccNiTghtmare is public domain. Do whatever you want with this code.

CPP = g++
CC = gcc
CFLAGS = -g -Wall

all: glm.o fifo.o texture.o

texture.o: texture.h texture.cpp
	$(CPP) $(CFLAGS) -c texture.cpp -o texture.o

glm.o: glm.h glm.c
	$(CPP) $(CFLAGS) -c glm.c -o glm.o

fifo.o: fifo.c fifo.h
	$(CPP) $(CFLAGS) -c fifo.c -o fifo.o

fifo: fifo.c fifo.h
	$(CPP) $(CFLAGS) -DFIFO_TEST fifo.c -o fifo

clean:
	\rm -rf *.o fifo
