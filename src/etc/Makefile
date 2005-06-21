# DccNiTghtmare is public domain. Do whatever you want with this code.

CC = g++
CFLAGS = -g -Wall

all: glm.o fifo.o

glm.o: glm.h glm.c
	$(CC) $(CFLAGS) -c glm.c -o glm.o

fifo.o: fifo.c fifo.h
	$(CC) $(CFLAGS) -c fifo.c -o fifo.o

fifo: fifo.c fifo.h
	$(CC) $(CFLAGS) -DFIFO_TEST fifo.c -o fifo

clean:
	\rm -rf *.o fifo
