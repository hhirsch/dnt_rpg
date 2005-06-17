#ifndef _NETWORK_H
#define _NETWORK_H

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

#define DEFAULTPORT 9000
#define MAXCLIENTS 10
#define LISTENTIMEOUT 10
#define TIMEOUT 180
#define BUFFERSIZE 1024
#define BACKLOG 10

#endif
