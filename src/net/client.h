#ifndef _CLIENT_H
#define _CLIENT_H

#include "common.h"

int initclientdata( clientdata_p_t cd );

int startconnection( clientdata_p_t cd, char * server, int port );

int closeconnection( clientdata_p_t cd );

netevent_p_t pollnet( clientdata_p_t cd );

void entergame( clientdata_p_t cd );

int movchar( clientdata_p_t cd, int obj, double x, double y, double teta );

int createchar( clientdata_p_t cd, double x, double y, double teta );

#endif
