# DccNiTghtmare is public domain. Do whatever you want with this code.

CC = g++
CFLAGS = -g -c -Wall

all: glm.o

glm.o: glm.h glm.c
	$(CC) $(CFLAGS) glm.c -o glm.o

clean:
	\rm -rf *.o
