#include "network.h"

int start_connection( char * server, int port )
{
	int socketfd;
	struct sockaddr serveraddr;
	struct sockaddr_in * addr_in;
	struct hostent * host;
	
	addr_in = (struct sockaddr_in *)&serveraddr;
	if( ! inet_aton( server, &(addr_in->sin_addr) ) )
	{
		printf("Resolving %s... ", server);
		fflush(NULL);
		if ( (host = gethostbyname( server ) ) == NULL )
		{
			printf("Error\n");
			herror("gethostbyname");
			fprintf( stderr, "Couldn't resolv host name %s. Aborting.\n", server);
			return(-1);
		}
		printf( "%s\n", inet_ntoa(*((struct in_addr *)host->h_addr)));
		addr_in->sin_addr = *((struct in_addr *)host->h_addr);
	}
	addr_in->sin_family = AF_INET;
	addr_in->sin_port = htons(port);
	memset( &(addr_in->sin_zero), '\0', 8);
	if ( (socketfd = socket( AF_INET, SOCK_STREAM, 0 )) == -1 )
	{
		perror("Couldn't get socket descriptor. Aborting.\n");
		return(-1);
	}
	printf("Connecting to server %s ( port %d ) ", inet_ntoa(*((struct in_addr *)host->h_addr)), port);
	if ( connect(socketfd, &serveraddr, sizeof( struct sockaddr )) == -1 )
	{
		printf("Error.\n");
		perror("connect");
		return(errno);
	}
	printf(": Connected\n");
	return( socketfd );
}
	
int send_data( int socketfd, void * data, int datasize )
{
	int sent = 0;
	int n;
	
	while( sent < datasize )
	{
		n = send( socketfd, data + sent, datasize - sent, 0);
		if( n < 0 ){ break; }
		sent += n;
	}
	return(0);
}

int close_connection( int socketfd )
{
	printf("Disconnecting... ");
	if ( close( socketfd ) == -1 ){
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
	int fd;
	
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
	if ( (fd = start_connection( server, port )) == -1 )
	{
		exit(1);
	}
	else
	{
		printf("Aeeeeeh !!!\n");
		close_connection( fd );
	}
	return(0);
}
#endif
