/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

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

#include "../etc/fifo.h"

#define DEFAULTPORT 9001
#define MAXCLIENTS 10
#define LISTENTIMEOUT 10
#define TIMEOUT 180
#define BUFFERSIZE 4096
#define BACKLOG 10
#define MAXEVENTQUEUE 1000

/* This will be in gamedata.h or something like that in near (?) future: */
typedef struct _pc_t
{
	int stat;
	double x, y, teta;
} pc_t, * pc_p_t;

typedef struct _netevent_t
{
	int type;
	int obj;
	int aux;
	double x, y, teta;
} netevent_t, * netevent_p_t;

typedef struct _serverdata_t 
{
	int port;
	int listenerfd;
	int numclients;
	int hoststat [MAXCLIENTS + 1];
	int acks [MAXCLIENTS + 1];
	struct sockaddr addresses [MAXCLIENTS + 1];
	struct pollfd fdset [MAXCLIENTS + 1];
	void * outbuffer;
	void * inbuffer;
	int inlen, inoffset;
	int outlen, outoffset;
	// This will be in gamedata in near (?) future:
	pc_t pcs [MAXCLIENTS + 1];
} serverdata_t, *serverdata_p_t;

typedef struct _clientdata_t
{
	int stat;
	struct hostent * host;
	struct sockaddr serveraddr;
	struct pollfd fdset;
	void * outbuffer;
	void * inbuffer;
	int inlen, inoffset;
	int outlen, outoffset;
	int pending;
	fifo_t eventfifo;
// This will be in gamedata in near (?) future:
	int pcindex;
} clientdata_t, *clientdata_p_t;


/* Player character status flags: */
#define PCSTAT_OFF 0
#define PCSTAT_ON 1

/* Remote host status flags: */
#define STAT_OFFLINE 0
#define STAT_ONLINE 1
#define STAT_WAITING 2
#define STAT_ACKING 4
#define STAT_UNSYNC 8
#define STAT_SYNCING 16

extern int addrlen;

/* Number of msg types ( if you change above, change here to: ) */
#define NUMMT 7
/* Msg types: */
#define MT_ACK 0
#define MT_NEWCHAR 1
#define MT_MOV 2
#define MT_ERROR 3
#define MT_SYNC 4
#define MT_ENDSYNC 5
#define MT_CLOSE 6

/* MT_ERROR error types: */
#define MT_ERROR_UNSYNC 0
#define MT_ERROR_DOUBLECHAR 1
#define MT_ERROR_NOCHAR 2
#define MT_ERROR_OTHER 3

#define MT_STYPES { "i", "ddd", "ddd", "i", "", "", "" }

#define MT_SIZE_ACK (sizeof(int)*3)
#define MT_SIZE_NEWCHAR ((sizeof(int)*2)+(sizeof(double)*3))
#define MT_SIZE_MOV ((sizeof(int)*2)+(sizeof(double)*3))
#define MT_SIZE_ERROR (sizeof(int)*3)
#define MT_SIZE_SYNC (sizeof(int)*2)
#define MT_SIZE_ENDSYNC (sizeof(int)*2)
#define MT_SIZE_CLOSE (sizeof(int)*2)

extern const char * _mt_numargs[NUMMT];

int buildmesg( void * data, int type, int obj, ... );

int senddata( int socketfd, void * data, int datasize );

#endif
