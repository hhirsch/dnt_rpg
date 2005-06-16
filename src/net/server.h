#ifndef _SERVER_H
#define _SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define DEFAULTPORT 9000
#define MAXCLIENTS 10
#define LISTENTIMEOUT 10
#define TIMEOUT 180
#define BUFFERSIZE 1024

#endif
