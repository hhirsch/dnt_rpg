#include "common.h"

int initserverdata( serverdata_p_t sd )
{
	int i;
	if (( sd->outbuffer = (void*) malloc( BUFFERSIZE + 1 )) == NULL ) return(1);
	if (( sd->inbuffer = (void*) malloc( BUFFERSIZE + 1 )) == NULL ) return(1);
	sd->outlen = 0;
	sd->inlen = 0;
	sd->outoffset = 0;
	sd->inoffset = 0;
	sd->port = DEFAULTPORT;
	sd->numclients = 0;
	for( i = 0; i < MAXCLIENTS; i++ )
	{
		sd->hoststat[i] = STAT_OFFLINE;
		sd->pcs[i].stat = PCSTAT_OFF;
	}
	return(0);
}

void bcastmesg( serverdata_p_t sd, int sindex )
{
	int * iaux = (int*)(((char *)sd->inbuffer) + sd->inoffset);
	int i;
	sd->outlen = buildmesg( sd->outbuffer, MT_ACK, iaux[1], iaux[0] );
	for( i = 1; i <= sd->numclients; i++ )
	{
		if ( i == sindex )
		{
			senddata( sd->fdset[i].fd, sd->outbuffer, sd->outlen );
		}
		else
		{
			senddata( sd->fdset[i].fd, (void *)iaux, sd->inlen );
		}
	}
	return;
}

void sendstate( serverdata_p_t sd, int index )
{
	int i;
	for ( i = 1; i <= MAXCLIENTS; i++ )
	{
		if( sd->pcs[i].stat & PCSTAT_ON) 
		{
			sd->outlen = buildmesg( sd->outbuffer, MT_NEWCHAR, i, sd->pcs[i].x, sd->pcs[i].y, sd->pcs[i].teta );
			senddata( sd->fdset[index].fd, sd->outbuffer, sd->outlen );
			sd->acks[index]++;
		}
	}
	sd->outlen = buildmesg( sd->outbuffer, MT_ENDSYNC, 0 );
	senddata( sd->fdset[index].fd, sd->outbuffer, sd->outlen );
	sd->acks[index]++;
	sd->hoststat[index] |= (STAT_ACKING | STAT_UNSYNC );
}

void handlemesg( serverdata_p_t sd, int index )
{
	int * iaux = (int*)(((char *)sd->inbuffer) + sd->inoffset);
	double * daux = (double *) &(iaux[2]);
	struct sockaddr_in * addr_in = (struct sockaddr_in *)&(sd->addresses[index]);
//	printf("Mesg size = %d\n", sd->inlen );
//	printf("Buffer offset = %d\n", sd->inoffset );
	switch( iaux[0] )
	{
		/* MT_ACK */
		case MT_ACK:
			// printf("MT_ACK received.\n");
			if( sd->acks[index] > 0 )
			{
				sd->acks[index] --;
				if ( sd->acks[index] == 0 )
					sd->hoststat[index] &= ~(STAT_ACKING);
				if ( iaux[2] == MT_ENDSYNC )
					sd->hoststat[index] &= ~(STAT_UNSYNC);
			}
			else
			{
				fprintf( stderr, "Unexpected ACK from host %s.\n", inet_ntoa( addr_in->sin_addr ));
				return;
			}
			sd->inlen -= MT_SIZE_ACK;
			sd->inoffset += MT_SIZE_ACK;
			break;

		/* MT_NEWCHAR */
		case MT_NEWCHAR:
			printf("MT_NEWCHAR received.\n");
			if( sd->hoststat[index] & STAT_UNSYNC )
			{
				fprintf( stderr, "Character creation from unsynced host %s.\n", inet_ntoa( addr_in->sin_addr ));
				sd->outlen = buildmesg( sd->outbuffer, MT_ERROR, iaux[1], MT_ERROR_UNSYNC );
				senddata( sd->fdset[index].fd, sd->outbuffer, sd->outlen);
				return;
			}
			if( sd->pcs[index].stat & PCSTAT_ON )
			{
				fprintf( stderr, "Another character creation requisition from host %s.\n", inet_ntoa( addr_in->sin_addr ));
				sd->outlen = buildmesg( sd->outbuffer, MT_ERROR, iaux[1], MT_ERROR_DOUBLECHAR );
				senddata( sd->fdset[index].fd, sd->outbuffer, sd->outlen);
				return;
			}
			sd->pcs[index].stat = PCSTAT_ON;
			sd->pcs[index].x = daux[0];
			sd->pcs[index].y = daux[1];
			sd->pcs[index].teta = daux[2];
			iaux[1] = index;
			bcastmesg( sd, index );
			sd->inlen -= MT_SIZE_NEWCHAR;
			sd->inoffset += MT_SIZE_NEWCHAR;
			break;
		case MT_SYNC:
			printf("MT_SYNC received.\n");
			sd->outlen = buildmesg( sd->outbuffer, MT_ACK, 0, MT_SYNC );
			senddata( sd->fdset[index].fd, sd->outbuffer, sd->outlen);
			sendstate( sd, index );
			sd->inlen -= MT_SIZE_SYNC;
			sd->inoffset += MT_SIZE_SYNC;
			break;
		case MT_MOV:
			// printf("MT_MOV received.\n");
			if( sd->hoststat[index] & STAT_UNSYNC )
			{
				fprintf( stderr, "Character movement from unsynced host %s.\n", inet_ntoa( addr_in->sin_addr ));
				sd->outlen = buildmesg( sd->outbuffer, MT_ERROR, iaux[1], MT_ERROR_UNSYNC );
				senddata( sd->fdset[index].fd, sd->outbuffer, sd->outlen);
				return;
			}
			if( ! (sd->pcs[index].stat & PCSTAT_ON) )
			{
				fprintf( stderr, "Host %s trying to move unexistent character.\n", inet_ntoa( addr_in->sin_addr ));
				sd->outlen = buildmesg( sd->outbuffer, MT_ERROR, iaux[1], MT_ERROR_NOCHAR );
				senddata( sd->fdset[index].fd, sd->outbuffer, sd->outlen);
				return;
			}
			if( iaux[1] != index )
			{
				fprintf( stderr, "Host %s trying to move another host character.\n", inet_ntoa( addr_in->sin_addr ));
				sd->outlen = buildmesg( sd->outbuffer, MT_ERROR, iaux[1], MT_ERROR_OTHER );
				senddata( sd->fdset[index].fd, sd->outbuffer, sd->outlen);
				return;
			}
			sd->pcs[index].x = daux[0];
			sd->pcs[index].y = daux[1];
			sd->pcs[index].teta = daux[2];
			bcastmesg( sd, index );
			sd->inlen -= MT_SIZE_MOV;
			sd->inoffset += MT_SIZE_MOV;
			break;
	}
	return;
}

int initlisten( serverdata_p_t sd )
{
	struct sockaddr_in * addr_in;
	int yes = 1;

	if ( (sd->listenerfd = socket( AF_INET, SOCK_STREAM, 0 )) == -1 )
	{
		perror( "socket" );
		return(errno);
	}
	fcntl( sd->listenerfd, F_SETFL, 0 );
	if ( setsockopt( sd->listenerfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1 )
	{
		perror("setsockopt");
		return(errno);
	}
	addr_in = (struct sockaddr_in *) &(sd->addresses[0]);
	addr_in->sin_family = AF_INET;
	addr_in->sin_port = htons( sd->port );
	addr_in->sin_addr.s_addr = INADDR_ANY;
	memset( &(addr_in->sin_zero), '\0', 8);
	if ( bind( sd->listenerfd, &(sd->addresses[0]), addrlen ) != 0 )
	{
		perror("bind");
		return(errno);
	}
	if ( listen( sd->listenerfd, BACKLOG ) == -1 )
	{
		perror("listen");
		return(errno);
	}
	printf("DCCNitghtmare Server v.0.0.0.1\n\nListening for connections ( max %d clients ) on port %d\n", MAXCLIENTS, sd->port );
	sd->fdset[0].fd = sd->listenerfd;
	sd->fdset[0].events = POLLIN | POLLPRI ;
	sd->fdset[0].revents = 0;
	return(0);
}

int mainloop( serverdata_p_t sd )
{
	int newfd, pollret, i, j;
	struct sockaddr_in * addr_in;

	while( 1 )
	{
		pollret = poll( sd->fdset, sd->numclients + 1, (LISTENTIMEOUT * 1000) );
		if ( pollret > 0 )
		{
			for ( i = 0; i <= sd->numclients; i++ )
			{
				if ( sd->fdset[i].revents & ( POLLERR | POLLHUP | POLLNVAL ) )
				{
					addr_in = (struct sockaddr_in *) &(sd->addresses[i]);
					printf( "Hungup connection with client ( host %s ).\n", inet_ntoa( addr_in->sin_addr ));
					close(sd->fdset[i].fd);
					for( j = i; j < sd->numclients; j++ )
						sd->fdset[j] = sd->fdset[j+1];
					sd->numclients--;
					sd->hoststat[i] = STAT_OFFLINE;
					sd->pcs[i].stat = PCSTAT_OFF;
					i--;
				}
				else if ( sd->fdset[i].revents & (POLLIN | POLLPRI))
				{
					if ( sd->fdset[i].fd == sd->listenerfd )
					{
						if ( sd->numclients >= MAXCLIENTS )
						{
							printf( "New connection atempt arrived, but already at max client number.\n" );
						}
						else
						{
							printf( "New connection attempt.\n" );
							if (( newfd = accept( sd->listenerfd, &(sd->addresses[sd->numclients + 1]), (socklen_t *)&addrlen )) == -1 )
							{
								printf( "Could not stabilish the connection with remote host.\n" );
							}
							else
							{
								printf( "Connection stabilished with remote host (fd %d).\n", newfd );
								sd->numclients++;
								sd->hoststat[i] = STAT_ONLINE | STAT_UNSYNC;
								sd->fdset[sd->numclients].fd = newfd;
								sd->fdset[sd->numclients].events = POLLIN | POLLPRI ;
								sd->fdset[sd->numclients].revents = 0;
							}
						}
					}
					else
					{
						if( (sd->inlen = recv( sd->fdset[i].fd, sd->inbuffer, BUFFERSIZE, 0)) == 0 )
						{
							addr_in = (struct sockaddr_in *) &(sd->addresses[i]);
							printf( "Connection closed by client ( host %s ).\n", inet_ntoa( addr_in->sin_addr ));
							close(sd->fdset[i].fd);
							for( j = i; j < sd->numclients; j++ )
								sd->fdset[j] = sd->fdset[j+1];
							sd->numclients--;
							sd->hoststat[i] = STAT_OFFLINE;
							sd->pcs[i].stat = PCSTAT_OFF;
							i--;
						}
						else if( sd->inlen == -1 )
						{
							perror("recv");
							close(sd->fdset[i].fd);
							for( j = i; j < sd->numclients; j++ )
								sd->fdset[j] = sd->fdset[j+1];
							sd->numclients--;
							sd->hoststat[i] = STAT_OFFLINE;
							sd->pcs[i].stat = PCSTAT_OFF;
							i--;
						}
						while( sd->inlen > 0 )
						{
							handlemesg( sd, i );
						}
						sd->inoffset = 0;
					}
				}
			}
		}
		/*
		   else if ( pollret == 0 )
		   {
		   printf("Timeout...\n");
		   }*/
		else if ( pollret < 0 )
		{
			perror("poll");
		}
	}
	close(sd->listenerfd);
	return(0);
}

int main( int argc, char ** argv )
{
	char opt;
	serverdata_t sd;


	while(( opt = getopt( argc, argv, "p:" )) != -1 )
	{
		switch( opt )
		{
			case 'p':
				sd.port = atoi( optarg );
				//printf( "Set listen port to %d.\n", sd.port );
				break;
			case ':':
				return(1);
				break;
			case '?':
				return(1);
				break;
		}
	}

	initserverdata( &sd );
	if (initlisten( &sd )) return(1);
	mainloop( &sd );
	return(0);
}

