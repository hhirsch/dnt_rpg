#include "network.h"
#include <fcntl.h>

void handlemesg( int size, void * buffer )
{
	printf("%s\n", (char *) buffer);
	return;
}

/* void bcastmesg( int size, void * buffer )*/

int main( int argc, char ** argv )
{
	char opt;
	void * buffer;
	int port = DEFAULTPORT, listenerfd, numclients = 0, newfd, pollret, i, j, numbytes, saddrsize = sizeof(struct sockaddr);
	struct sockaddr myaddr, *clients;
	struct sockaddr_in * addr_in;
	struct pollfd readfdset [MAXCLIENTS + 1];

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
				return(1);
				break;
			case '?':
				return(1);
				break;
		}
	}
	if ( (listenerfd = socket( AF_INET, SOCK_STREAM, 0 )) == -1 )
	{
		fprintf( stderr, "Error openning socket. Aborting\n" );
		return(errno);
	}
	fcntl( listenerfd, F_SETFL, 0 );
	addr_in = (struct sockaddr_in *) &myaddr;
	addr_in->sin_family = AF_INET;
	addr_in->sin_port = htons( port );
	addr_in->sin_addr.s_addr = INADDR_ANY;
	memset( &(addr_in->sin_zero), '\0', 8);
	printf( "Binding socket %d to port %d\n", listenerfd, port );
	if ( bind( listenerfd, &myaddr, sizeof( struct sockaddr )) != 0 )
	{
		fprintf( stderr, "Error binding socket to port %d. Aborting.\n", port );
		return(errno);
	}
	if ( listen( listenerfd, BACKLOG ) == -1 )
	{
		perror("listen");
		return(errno);
	}
	
	readfdset[0].fd = listenerfd;
	readfdset[0].events = POLLIN | POLLPRI ;
	readfdset[0].revents = 0;
	while( 1 )
	{
		pollret = poll( readfdset, numclients + 1, (LISTENTIMEOUT * 1000) );
		if ( pollret > 0 )
		{
			printf("Something happened...\n");
			for ( i = 0; i <= numclients; i++ )
			{
				if ( readfdset[i].revents & ( POLLERR | POLLHUP | POLLNVAL ) )
				{
					addr_in = (struct sockaddr_in *) &(clients[i]);
					printf( "Hungup connection with client ( host %s ).\n", inet_ntoa( addr_in->sin_addr ));
					close(readfdset[i].fd);
					for( j = i; j < numclients; j++ )
						readfdset[j] = readfdset[j+1];
					numclients--;
					i--;
				}
				else if ( readfdset[i].revents & (POLLIN | POLLPRI))
				{
					if ( readfdset[i].fd == listenerfd )
					{
						if ( numclients >= MAXCLIENTS )
						{
							fprintf( stderr, "New connection atempt arrived, but already at max client number.\n" );
						}
						else
						{
							printf( "New connection attempt.\n" );
							if (( newfd = accept( listenerfd, &(clients[numclients]), &saddrsize )) == -1 )
							{
								fprintf( stderr, "Could not stabilish the connection with remote host.\n" );
							}
							else
							{
								numclients++;
								readfdset[numclients].fd = newfd;
								readfdset[numclients].events = POLLIN | POLLPRI ;
								readfdset[numclients].revents = 0;
							}
						}
					}
					else
					{
						if( (numbytes = recv( readfdset[i].fd, buffer, BUFFERSIZE, 0)) == 0 )
						{
							addr_in = (struct sockaddr_in *) &(clients[i]);
							printf( "Connection closed by client ( host %s ).\n", inet_ntoa( addr_in->sin_addr ));
							close(readfdset[i].fd);
							for( j = i; j < numclients; j++ )
								readfdset[j] = readfdset[j+1];
							numclients--;
							i--;
						}
						else if( numbytes > 0 )
						{
							handlemesg( numbytes, buffer );
						}
						else if( numbytes == -1 )
						{
							perror("recv");
							close(readfdset[i].fd);
							for( j = i; j < numclients; j++ )
								readfdset[j] = readfdset[j+1];
							numclients--;
							i--;
						}
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
	close(listenerfd);
return(0);
}
