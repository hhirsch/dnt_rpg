#include "common.h"
// TODO: linha 14

int initclientdata( clientdata_p_t cd )
{
	cd->buffer = malloc( BUFFERSIZE );
	cd->buflen = 0;
	return(0);
}

int handlemesg( clientdata_p_t cd )
{
	int * iaux, * type, * obj;
	char * caux;
	double * daux;
	void * aux = cd->buffer;
	iaux = aux;
	switch ( iaux[0] )
	{
		case MT_NEWCHAR:
			// character creation function call;
			

int startconnection( clientdata_p_t cd, char * server, int port )
{
	struct sockaddr_in * addr_in;
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
	if ( (cd->fddata.fd = socket( AF_INET, SOCK_STREAM, 0 )) == -1 )
	{
		perror("Couldn't get socket descriptor. Aborting.\n");
		return(-1);
	}
	cd->fddata.events = POLLIN | POLLPRI;
	if ( setsockopt( cd->fddata.fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1 )
	{
		perror("setsockopt");
		return(-1);
	}
	fcntl( sd->listenerfd, F_SETFL, 0 );
	printf("Connecting to server %s ( port %d ) ", inet_ntoa((struct in_addr)addr_in->sin_addr), port);
	if ( connect(cd->fddata.fd, &(cd->serveraddr), sizeof( struct sockaddr )) == -1 )
	{
		printf("Error.\n");
		perror("connect");
		return(-1);
	}
	printf(": Connected\n");
	return( 0 );
}
	
int pollnet( clientdata_p_t cd )
{
	int pollret;

	cd->fddata.revents = 0;
	
	pollret = poll( &(cd->fddata), 1, 0 );
	if ( pollret > 0 )
	{
		cd->buflen = recv( sd->readfdset[i].fd, sd->buffer, BUFFERSIZE, 0);
		if ( cd->buflen == 0 )
		{
			printf("Connection closed by server.\n");
			closeconnection( cd );
			return(-1);
		}
		else if ( cd->buflen > 0 )
		{
			handlemesg( sd );
		}
		else
		{
			printf("Connection error with server.\n");
			closeconnection( cd );
			return(-1);
		}
		return(1);
	}
	return(0);
}

int closeconnection( clientdata_p_t cd )
{
	printf("Disconnecting... ");
	if ( close( cd->fddata.fd ) == -1 ){
		printf("Error.\n");
		return(1);
	}
	printf( "Done.\n" );
	return(0);
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
	mesg = mesgbuffer;
	
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
		printf("Aeeeeeh !!!\n");
		sprintf(mesg, "Ate que enfim consegui fala coce, po !\n");
		senddata( fd, mesg, strlen( mesg )+1 );
		sleep(20);
		sprintf(mesg, "To vazando !\n");
		senddata( fd, mesg, strlen( mesg )+1 );

		closeconnection( fd );
	}
	return(0);
}
#endif
