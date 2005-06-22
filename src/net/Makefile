CC=gcc
CPP=g++
CLIBS=
CLIBSDIR=
INCLUDESDIR=-I../etc
CFLAGS= -Wall -g ${INCLUDESDIR} ${CLIBS} ${CLIBSDIR}
BINDIR=../../bin

all: server client.o

common.o: common.c common.h
	${CPP} ${CFLAGS} -c common.c

client.o: common.o client.c common.h
	make -C ../etc fifo.o
	${CPP} ${CFLAGS} -o client.o -c client.c

server.o: server.c common.h
	${CPP} ${CFLAGS} -c server.c

server: server.o common.o
	${CPP} ${CFLAGS} -o server server.o common.o
	cp server ${BINDIR}

client: common.o server client.o common.h
	${CPP} ${CFLAGS} -c -DCLIENT_TEST client.c
	${CPP} ${CFLAGS} -DCLIENT_TEST ../etc/fifo.o client.o common.o -o client

clean:
	rm -rf server server.o common.o client.o client

run: server client
	./server
