#include "server.h"

void handlemesg( int size, void * buffer )
{
	printf("%s\n", (char *) buffer);
	return;
}

int main( int argc, char ** argv )
{
	char opt, *bufreader;
	void * buffer;
	int port = DEFAULTPORT, listenerfd, numclients = 0, fdmax, newfd, pollret, i, j, numbytes, saddrsize = sizeof(struct sockaddr);
	struct sockaddr myaddr, *clients, *addr;
	struct sockaddr_in * addr_in;
	struct timeval timeout;
	struct pollfd safefdset [MAXCLIENTS + 1], readfdset [MAXCLIENTS + 1];

	buffer = (void*) malloc( BUFFERSIZE + 1 );
    clients = (void*) malloc( sizeof( struct sockaddr )*MAXCLIENTS );

	while(( opt = getopt( argc, argv, "p:" )) != -1 )
	{
		switch( opt )
		{
			case 'p':
				port = atoi( optarg );
				printf( "Set listen port to %d.\n", port );
				break;
			case ':':
				exit(1);
				break;
			case '?':
				exit(1);
				break;
		}
	}
	if ( (listenerfd = socket( AF_INET, SOCK_STREAM, 0 )) == -1 )
	{
		fprintf( stderr, "Error openning socket. Aborting\n" );
		exit(1);
	}
	addr_in = (struct sockaddr_in *) &myaddr;
	addr_in->sin_family = AF_INET;
	addr_in->sin_port = htons( port );
	addr_in->sin_addr.s_addr = INADDR_ANY;
	memset( &(addr_in->sin_zero), '\0', 8);
	if ( bind( listenerfd, &myaddr, sizeof( struct sockaddr )) == -1 )
	{
		fprintf( stderr, "Error binding socket to port %d. Aborting.\n", port );
		exit(errno);
	}
	safefdset[0].fd = listenerfd;
	safefdset[0].events = POLLIN | POLLPRI ;//| POLLHUP;
	safefdset[0].revents = 0;
	while( 1 )
	{
		for( i = 0; i <= numclients; i++ )
			readfdset[i] = safefdset[i];

		timeout.tv_sec = LISTENTIMEOUT;
		if ( ( pollret = poll( readfdset, numclients + 1, LISTENTIMEOUT * 1000 )) != -1 )
		{
			if ( pollret > 0 )
			{
				for ( i = 0; i <= numclients; i++ )
				{
					if ( readfdset[i].revents & (POLLIN | POLLPRI))
					{
						if ( i == listenerfd )
						{
							if ( numclients >= MAXCLIENTS )
							{
								fprintf( stderr, "New connection atempt arrived, but already at max client number.\n" );
							}
							else
							{
								printf( "New connection attempt ( in socket %d; listener is %d).\n", i, listenerfd );
								if (( newfd = accept( listenerfd, &(clients[numclients]), &saddrsize )) == -1 )
								{
									fprintf( stderr, "Could not stabilish the connection with remote host.\n" );
								}else{
									safefdset[i].fd = newfd;
									safefdset[i].events = POLLIN | POLLPRI ;//| POLLHUP;
									safefdset[i].revents = 0;
									if( fdmax < newfd )
										fdmax = newfd;
								}
							}
						}
						else
						{
							if( (numbytes = recv( i, buffer, BUFFERSIZE, 0)) == 0 )
							{
								addr_in = (struct sockaddr_in *) &(clients[i]);
								printf( "Connection closed by client ( host %s ).\n", inet_ntoa( addr_in->sin_addr ));
								close(i);
								// FD_CLR(i, &safefdset );
							}
							else if( numbytes > 0 )
							{
								handlemesg( numbytes, buffer );
							}
							else
							{
								fprintf(
										stderr,
										"%s %d, %s: Error receiving data.\n",
										__FILE__,
										__LINE__,
										__FUNCTION__
									   );
							}
						}
					}
					/*
					else if ( readfdset[i].revents & POLLHUP )
					{
						addr_in = (struct sockaddr_in *) &(clients[i]);
						printf( "Hungup connection with client ( host %s ).\n", inet_ntoa( addr_in->sin_addr ));
						close(i);
						// FD_CLR(i, &safefdset );
					}*/
				}
			}
		}
	}
	return(0);
}
