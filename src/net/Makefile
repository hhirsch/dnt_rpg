CC=gcc
CLIBS=
CLIBSDIR=
INCLUDESDIR=-I../etc
CFLAGS= -Wall -g ${INCLUDESDIR} ${CLIBS} ${CLIBSDIR}

all: server client.o

common.o: common.c common.h
	${CC} ${CFLAGS} -c common.c

client.o: common.o client.c common.h
	make -C ../etc fifo.o
	${CC} ${CFLAGS} -o client.o -c client.c

server.o: server.c common.h
	${CC} ${CFLAGS} -c server.c

server: server.o common.o
	${CC} ${CFLAGS} -o server server.o common.o

client: common.o server client.c common.h
	${CC} ${CFLAGS} -c -DCLIENT_TEST client.c
	${CC} ${CFLAGS} -DCLIENT_TEST client.o common.o ../etc/fifo.o -o client

clean:
	rm -rf server server.o common.o client.o client

run: server client
	./server
