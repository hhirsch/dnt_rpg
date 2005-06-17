CC=gcc
CFLAGS= -Wall -g
CLIBS=
CLIBSDIR=

all: server client.o

common.o: common.c common.h
	${CC} ${CFLAGS} -c common.c

client.o: common.o client.c common.h
	${CC} ${CFLAGS} -c client.c
	${CC} ${CFLAGS} -o client client.o common.o

server.o: server.c common.h
	${CC} ${CFLAGS} -c server.c

server: server.o common.o
	${CC} ${CFLAGS} -o server server.o common.o

client: common.o server client.c common.h
	${CC} ${CFLAGS} -c -DCLIENT_TEST client.c
	${CC} ${CFLAGS} -DCLIENT_TEST client.o common.o -o client

clean:
	rm -rf server server.o common.o client.o client

run: server client
	./server
