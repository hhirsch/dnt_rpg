#include "client.h"

int initclientdata( clientdata_p_t cd )
{
	fifoinit( &(cd->eventfifo), MAXEVENTQUEUE );
	cd->inbuffer = malloc( BUFFERSIZE );
	cd->outbuffer = malloc( BUFFERSIZE );
	cd->inlen = 0;
	cd->outlen = 0;
	cd->inoffset = 0;
	cd->outoffset = 0;
	cd->stat = STAT_OFFLINE;
	cd->pending = -1;
	return(0);
}

int handlemesg( clientdata_p_t cd )
{
	int * iaux = (int*)(((char *)cd->inbuffer) + cd->inoffset);
	double * daux = (double*)&(iaux[2]);
	netevent_p_t eaux;
	
	printf("Mesg size = %d\n", cd->inlen );
	printf("Buffer offset = %d\n", cd->inoffset );
	switch ( iaux[0] )
	{
		case MT_NEWCHAR:
			printf("MT_NEWCHAR received.\n");
			
			eaux = (netevent_p_t)malloc( sizeof( netevent_t ) );
			eaux->type = iaux[0];
			eaux->obj = iaux[1];
			eaux->aux = -1;
			eaux->x = daux[0];
			eaux->y = daux[1];
			eaux->teta = daux[2];
			fifopush( &(cd->eventfifo), eaux );

			cd->outlen = buildmesg( cd->outbuffer, MT_ACK, iaux[1], MT_NEWCHAR );
			senddata( cd->fdset.fd, cd->outbuffer, cd->outlen );
			cd->inlen -= MT_SIZE_NEWCHAR;
			cd->inoffset += MT_SIZE_NEWCHAR;
			break;
		case MT_ACK:
			printf("MT_ACK received.\n");
			switch ( cd->pending )
			{
				case MT_NEWCHAR:
					if ( iaux[2] != MT_NEWCHAR )
					{
						fprintf(stderr, "Unexpected ACK received ( expecting MT_NEWCHAR ).\n");
						return(-1);
					}
					eaux = (netevent_p_t)malloc( sizeof( netevent_t ) );
					eaux->type = iaux[0];
					eaux->obj = -1;
					eaux->aux = MT_NEWCHAR;
					eaux->x = daux[0];
					eaux->y = daux[1];
					eaux->teta = daux[2];
					fifopush( &(cd->eventfifo), eaux );

					cd->pcindex = iaux[1]; 
					cd->pending = -1;
					break;
				case MT_MOV:
					if ( iaux[2] != MT_MOV )
					{
						fprintf(stderr, "Unexpected ACK received ( expecting MT_MOV ).\n");
						return(-1);
					}
					eaux = (netevent_p_t)malloc( sizeof( netevent_t ) );
					eaux->type = iaux[0];
					eaux->obj = iaux[1];
					eaux->aux = MT_MOV;
					eaux->x = daux[0];
					eaux->y = daux[1];
					eaux->teta = daux[2];
					fifopush( &(cd->eventfifo), eaux );

					cd->pending = -1;
					break;
				case MT_SYNC:
					if ( iaux[2] != MT_SYNC )
					{
						fprintf(stderr, "Unexpected ACK received ( expecting MT_SYNC ).\n");
						return(-1);
					}
					cd->pending = MT_ENDSYNC;
					break;
			}
			cd->inlen -= MT_SIZE_ACK;
			cd->inoffset += MT_SIZE_ACK;
			break;
		case MT_MOV:
			printf("MT_MOV received.\n");

			eaux = (netevent_p_t)malloc( sizeof( netevent_t ) );
			eaux->type = iaux[0];
			eaux->obj = iaux[1];
			eaux->aux = -1;
			eaux->x = daux[0];
			eaux->y = daux[1];
			eaux->teta = daux[2];
			fifopush( &(cd->eventfifo), eaux );

			cd->outlen = buildmesg( cd->outbuffer, MT_ACK, iaux[1], MT_MOV );
			senddata( cd->fdset.fd, cd->outbuffer, cd->outlen );
			cd->inlen -= MT_SIZE_MOV;
			cd->inoffset += MT_SIZE_MOV;
			break;
		case MT_ERROR:
			printf("MT_ERROR received.\n");
			switch ( iaux[2] )
			{
				case MT_ERROR_UNSYNC:
					cd->outlen = buildmesg( cd->outbuffer, MT_SYNC, 0 );
					senddata( cd->fdset.fd, cd->outbuffer, cd->outlen );
					cd->pending = MT_SYNC;
					cd->stat |= STAT_SYNCING;
					break;
				case MT_ERROR_DOUBLECHAR:
					fprintf(stderr, "I'm trying to create two playable characters on the server. Why ?\n");
					break;
				case MT_ERROR_NOCHAR:
					fprintf(stderr, "I'm trying to move an unexistent character in the server. Why ?\n");
					break;
			}
			eaux = (netevent_p_t)malloc( sizeof( netevent_t ) );
			eaux->type = iaux[0];
			eaux->obj = iaux[1];
			eaux->aux = iaux[2];
			fifopush( &(cd->eventfifo), eaux );
			
			cd->inlen -= MT_SIZE_ERROR;
			cd->inoffset += MT_SIZE_ERROR;
			break;
		case MT_ENDSYNC:
			printf("MT_ENDSYNC received.\n");
			if ( cd->pending != MT_ENDSYNC )
			{
				fprintf(stderr, "Unexpected MT_ENDSYNC received.\n");
				return(-1);
			}
			cd->outlen = buildmesg( cd->outbuffer, MT_ACK, iaux[1], MT_ENDSYNC );
			senddata( cd->fdset.fd, cd->outbuffer, cd->outlen );
			
			eaux = (netevent_p_t)malloc( sizeof( netevent_t ) );
			eaux->type = iaux[0];
			eaux->obj = iaux[1];
			eaux->aux = iaux[2];
			fifopush( &(cd->eventfifo), eaux );

			cd->pending = -1;
			cd->stat &= ~( STAT_SYNCING | STAT_UNSYNC );
			cd->inlen -= MT_SIZE_ENDSYNC;
			cd->inoffset += MT_SIZE_ENDSYNC;
			break;
	}
	return(0);
}

int startconnection( clientdata_p_t cd, char * server, int port )
{
	struct sockaddr_in * addr_in;
	//int * iaux = cd->inbuffer;
	int yes = 1;

	addr_in = (struct sockaddr_in *)&(cd->serveraddr);
	if( inet_aton( server, &(addr_in->sin_addr) ) == 0)
	{
		printf("Resolving %s... ", server);
		fflush(NULL);
		if ( (cd->host = gethostbyname( server ) ) == NULL )
		{
			printf("Error\n");
			herror("gethostbyname");
			fprintf( stderr, "Couldn't resolv host name %s. Aborting.\n", server);
			return(-1);
		}
		printf( "%s\n", inet_ntoa(*((struct in_addr *)cd->host->h_addr)));
		addr_in->sin_addr = *((struct in_addr *)cd->host->h_addr);
	}
	addr_in->sin_family = AF_INET;
	addr_in->sin_port = htons(port);
	memset( &(addr_in->sin_zero), '\0', 8);
	if ( (cd->fdset.fd = socket( AF_INET, SOCK_STREAM, 0 )) == -1 )
	{
		perror("Couldn't get socket descriptor. Aborting.\n");
		return(-1);
	}
	cd->fdset.events = POLLIN | POLLPRI;
	if ( setsockopt( cd->fdset.fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1 )
	{
		perror("setsockopt");
		return(-1);
	}
	fcntl( cd->fdset.fd, F_SETFL, 0 );
	printf("Connecting to server %s ( port %d, through fd %d ) ", inet_ntoa((struct in_addr)addr_in->sin_addr), port, cd->fdset.fd );
	if ( connect(cd->fdset.fd, &(cd->serveraddr), sizeof( struct sockaddr )) == -1 )
	{
		printf("Error.\n");
		perror("connect");
		return(-1);
	}
	cd->stat = STAT_ONLINE | STAT_UNSYNC;
	printf(": Connected\n");
	return( 0 );
}

int closeconnection( clientdata_p_t cd )
{
	printf("Disconnecting... ");
	if ( close( cd->fdset.fd ) == -1 ){
		printf("Error.\n");
		return(1);
	}
	printf( "Done.\n" );
	return(0);
}

/* Isso faz o que vc queria, Farrer:
 * Da uma olhada em netevent_t em common.h
 *
 * Essa função retorna NULL se evento nenhum ocorreu, 
 * ou um apontador para o tipo netevent_t, cujo campo
 * type determina o tipo do evento ( MT_NEWCHAR, MT_MOV, MT_CLOSE ).
 */

netevent_p_t pollnet( clientdata_p_t cd )
{
	int pollret;
	netevent_p_t eaux;

	cd->fdset.revents = 0;

	pollret = poll( &(cd->fdset), 1, 0 );
	if ( pollret > 0 )
	{
		cd->inlen = recv( cd->fdset.fd, cd->inbuffer, BUFFERSIZE, 0);
		if ( cd->inlen == 0 )
		{
			printf("Connection closed by server.\n");
			closeconnection( cd );
			eaux = (netevent_p_t)malloc( sizeof( netevent_t) );
			eaux->type = MT_CLOSE;
			return(eaux);
		}
		else if ( cd->inlen < 0 )
		{
			printf("Connection error with server.\n");
			closeconnection( cd );
			eaux = (netevent_p_t)malloc( sizeof( netevent_t) );
			eaux->type = MT_CLOSE;
			return(eaux);
		}
		while ( cd->inlen > 0 )
		{
			handlemesg( cd );
		}
		cd->inoffset = 0;
	}
	return( (netevent_p_t) fifopop( &(cd->eventfifo) ) );
}

void entergame( clientdata_p_t cd )
{
	if (( cd->stat & STAT_UNSYNC ) && (! ( cd->stat & STAT_SYNCING )))
	{
		cd->outlen = buildmesg( cd->outbuffer, MT_SYNC, 0 );
		senddata( cd->fdset.fd, cd->outbuffer, cd->outlen );
		cd->pending = MT_SYNC;
		cd->stat |= STAT_SYNCING;
	}
}

int movchar( clientdata_p_t cd, int obj, double x, double y, double teta )
{
	if (( cd->stat & STAT_UNSYNC ) && (! ( cd->stat & STAT_SYNCING )))
	{
		cd->outlen = buildmesg( cd->outbuffer, MT_SYNC, 0 );
		senddata( cd->fdset.fd, cd->outbuffer, cd->outlen );
		cd->pending = MT_SYNC;
		cd->stat |= STAT_SYNCING;
	}
	else
	{
		cd->outlen = buildmesg( cd->outbuffer, MT_MOV, obj, x, y, teta );
		senddata( cd->fdset.fd, cd->outbuffer, cd->outlen );
		cd->pending = MT_ACK;
	}
	return(0);
}

int createchar( clientdata_p_t cd, double x, double y, double teta )
{
	netevent_p_t eaux;
	int iaux = (int*)cd->inbuffer;
	int pollret;
	
	if (( cd->stat & STAT_UNSYNC ) && (! ( cd->stat & STAT_SYNCING )))
	{
		return(-2);
	}
	else
	{
		cd->outlen = buildmesg( cd->outbuffer, MT_NEWCHAR, obj, x, y, teta );
		senddata( cd->fdset.fd, cd->outbuffer, cd->outlen );
	}
	while(1)
	{
		cd->inlen = recv( cd->fdset.fd, cd->inbuffer, BUFFERSIZE, 0);
		if ( cd->inlen == 0 )
		{
			printf("Connection closed by server.\n");
			closeconnection( cd );
			return(-1);
		}
		else if ( cd->inlen < 0 )
		{
			printf("Connection error with server.\n");
			closeconnection( cd );
			return(-1);
		}
		while ( cd->inlen > 0 )
		{
			handlemesg( cd );
		}
		cd->inoffset = 0;
		while( ( eaux = (netevent_p_t) fifopop( &(cd->eventfifo) )) != NULL )
		{
			if( eaux->type == MT_NEWCHAR )
			{
				return( iaux[1] );
			}
			else
			{
				cd->stat |= STAT_UNSYNC;
				return(-2);
			}
		}
	}
	return(-1);
}

#ifdef CLIENT_TEST

int main( int argc, char ** argv )
{
	char opt;
	int port = DEFAULTPORT;
	char * server;
	clientdata_t cd;
	void * mesgbuffer = malloc( BUFFERSIZE );
	char * mesg;
	netevent_p_t eaux;
	mesg = (char *)mesgbuffer;

	while(( opt = getopt( argc, argv, "p:" )) != -1 )
	{
		switch( opt )
		{
			case 'p':
				port = atoi( optarg );
				printf( "Set port to %d.\n", port );
				break;
			case ':':
				exit(1);
				break;
			case '?':
				exit(1);
				break;
		}
	}
	if( optind < argc )
	{
		server = argv[optind];
	}
	else
	{
		printf("Usage %s [-p <PORT>] <server>\n", argv[0]);
		exit(1);
	}
	initclientdata( &cd );
	if ( ( startconnection( &cd, server, port )) == -1 )
	{
		exit(1);
	}
	else
	{
		while(1)
		{
			entergame( &cd );
			eaux = pollnet( &cd );
			if((eaux != NULL ) && ( eaux->type == MT_CLOSE ))
			{
				closeconnection( &cd );
				break;
			}
			sleep(1);
		}
	}
	return(0);
}
#endif
