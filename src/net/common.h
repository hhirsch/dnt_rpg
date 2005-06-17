#ifndef _NETCOMMON_H
#define _NETCOMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>

#define DEFAULTPORT 9001
#define MAXCLIENTS 10
#define LISTENTIMEOUT 10
#define TIMEOUT 180
#define BUFFERSIZE 1024
#define BACKLOG 10

typedef struct _serverdata_t 
{
	int port;
	int listenerfd;
	int numclients;
	struct sockaddr myaddr;
	struct sockaddr *clients;
	struct pollfd readfdset [MAXCLIENTS + 1];
	void * buffer;
	int buflen;
} serverdata_t, *serverdata_p_t;

typedef struct _clientdata_t
{
	struct hostent * host;
	struct sockaddr serveraddr;
	struct pollfd fdset;
	void * buffer;
	int buflen;
} clientdata_t, *clientdata_p_t;

extern int addrlen;

/* Number of msg types ( if you change above, change here to: ) */
#define NUMMT 3
/* Msg types: */
#define MT_ACK 0
#define MT_NEWCHAR 1
#define MT_MOV 2

#define MT_STYPES { "i" "ddds", "ddd" }

extern const char * _mt_numargs[NUMMT];

int buildmesg( void * data, int type, int obj, ... );

int senddata( int socketfd, void * data, int datasize );

#endif
