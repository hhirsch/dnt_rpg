
CC=gcc
CFLAGS= -Wall
CLIBS=
CLIBSDIR=

all: server client.o

client.o: client.c network.h
	${CC} ${CFLAGS} -c client.c

server: server.c network.h
	${CC} ${CFLAGS} server.c -o server

test: client.c network.h server
	${CC} ${CFLAGS} -DCLIENT_TEST client.c -o client

clean:
	rm -rf server client.o client
