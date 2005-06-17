#include "common.h"

void handlemesg( serverdata_p_t sd, int senderindex )
{
	void * ackbuffer = malloc( BUFFERSIZE );
	int acksize, * iaux, i;
	iaux = sd->buffer;
	acksize = buildmesg( ackbuffer, MT_ACK, 0, iaux );
	for( i = 1; i <= sd->numclients; i++ )
	{
		if ( i == senderindex )
		{
			senddata( sd->readfdset[i].fd, ackbuffer, acksize );
		}
		else
		{
			senddata( sd->readfdset[i].fd, sd->buffer, sd->buflen );
		}
	}
	return;
}

/* void bcastmesg( int size, void * buffer )*/

int initserverdata( serverdata_p_t sd )
{
	if (( sd->buffer = (void*) malloc( BUFFERSIZE + 1 )) == NULL ) return(1);
    if (( sd->clients = (void*) malloc( sizeof( struct sockaddr )*MAXCLIENTS )) == NULL ) return(1);
	sd->buflen = 0;
	sd->port = DEFAULTPORT;
	sd->numclients = 0;
	return(0);
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
	addr_in = (struct sockaddr_in *) &(sd->myaddr);
	addr_in->sin_family = AF_INET;
	addr_in->sin_port = htons( sd->port );
	addr_in->sin_addr.s_addr = INADDR_ANY;
	memset( &(addr_in->sin_zero), '\0', 8);
	if ( bind( sd->listenerfd, &(sd->myaddr), addrlen ) != 0 )
	{
		perror("bind");
		return(errno);
	}
	if ( listen( sd->listenerfd, BACKLOG ) == -1 )
	{
		perror("listen");
		return(errno);
	}
	sd->readfdset[0].fd = sd->listenerfd;
	sd->readfdset[0].events = POLLIN | POLLPRI ;
	sd->readfdset[0].revents = 0;
	return(0);
}
	
int mainloop( serverdata_p_t sd )
{
	int newfd, pollret, i, j;
	struct sockaddr_in * addr_in;

	while( 1 )
	{
		pollret = poll( sd->readfdset, sd->numclients + 1, (LISTENTIMEOUT * 1000) );
		if ( pollret > 0 )
		{
			for ( i = 0; i <= sd->numclients; i++ )
			{
				if ( sd->readfdset[i].revents & ( POLLERR | POLLHUP | POLLNVAL ) )
				{
					addr_in = (struct sockaddr_in *) &(sd->clients[i]);
					printf( "Hungup connection with client ( host %s ).\n", inet_ntoa( addr_in->sin_addr ));
					close(sd->readfdset[i].fd);
					for( j = i; j < sd->numclients; j++ )
						sd->readfdset[j] = sd->readfdset[j+1];
					sd->numclients--;
					i--;
				}
				else if ( sd->readfdset[i].revents & (POLLIN | POLLPRI))
				{
					if ( sd->readfdset[i].fd == sd->listenerfd )
					{
						if ( sd->numclients >= MAXCLIENTS )
						{
							printf( "New connection atempt arrived, but already at max client number.\n" );
						}
						else
						{
							printf( "New connection attempt.\n" );
							if (( newfd = accept( sd->listenerfd, &(sd->clients[sd->numclients]), &addrlen )) == -1 )
							{
								printf( "Could not stabilish the connection with remote host.\n" );
							}
							else
							{
								sd->numclients++;
								sd->readfdset[sd->numclients].fd = newfd;
								sd->readfdset[sd->numclients].events = POLLIN | POLLPRI ;
								sd->readfdset[sd->numclients].revents = 0;
							}
						}
					}
					else
					{
						if( (sd->buflen = recv( sd->readfdset[i].fd, sd->buffer, BUFFERSIZE, 0)) == 0 )
						{
							addr_in = (struct sockaddr_in *) &(sd->clients[i]);
							printf( "Connection closed by client ( host %s ).\n", inet_ntoa( addr_in->sin_addr ));
							close(sd->readfdset[i].fd);
							for( j = i; j < sd->numclients; j++ )
								sd->readfdset[j] = sd->readfdset[j+1];
							sd->numclients--;
							i--;
						}
						else if( sd->buflen > 0 )
						{
							handlemesg( sd, i );
						}
						else if( sd->buflen == -1 )
						{
							perror("recv");
							close(sd->readfdset[i].fd);
							for( j = i; j < sd->numclients; j++ )
								sd->readfdset[j] = sd->readfdset[j+1];
							sd->numclients--;
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
				printf( "Set listen port to %d.\n", sd.port );
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

