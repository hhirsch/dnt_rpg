#include "common.h"
#include <stdarg.h>

int addrlen = sizeof(struct sockaddr);
const char * _mt_numargs[NUMMT] = MT_STYPES;

int buildmesg( void * data, int type, int obj, ... )
{
	va_list arglist;
	int numfields = strlen( _mt_numargs[type] );
	void * aux = data;
	int * iaux = (int*)aux;
	char * caux, * cauxaux;
	double * daux;
	int i;

	iaux[0] = type;
	iaux[1] = obj;
	aux = (void*)(&(iaux[2]));
	
	va_start( arglist, numfields );
	
	for( i = 0; i < numfields; i++ )
	{
		iaux = (int *)aux;
		caux = (char *)aux;
		daux = (double *)aux;
		switch( _mt_numargs[type][i] )
		{
			case 'i':
				*iaux = va_arg( arglist, int );
				aux = (void *)(&(iaux[1]));
				break;
			case 'd':
				*daux = va_arg( arglist, double );
				aux = (void *)(&(daux[1]));
				break;
			case 's':
				cauxaux = va_arg( arglist, char *);
				sprintf(caux, "%s", cauxaux);
				aux = (void *)(&(caux[strlen(cauxaux)+1]));
				break;
		}
	}
	va_end( arglist );
	caux = (char *) aux;
	cauxaux = (char *) data;
	return( caux - cauxaux );
}

int senddata( int socketfd, void * data, int datasize )
{
	int sent = 0;
	int n;
	int * iaux = (int *)data;
	char * caux = (char *)data;
/*	
	switch( iaux[0] )
	{
		case MT_ACK:
			printf("Sending MT_ACK to %d.\n", socketfd );
			break;
		case MT_NEWCHAR:
			printf("Sending MT_NEWCHAR to %d.\n", socketfd );
			break;
		case MT_MOV:
			printf("Sending MT_MOV to %d.\n", socketfd );
			break;
		case MT_ERROR:
			printf("Sending MT_ERROR to %d.\n", socketfd );
			break;
		case MT_SYNC:
			printf("Sending MT_SYNC to %d.\n", socketfd );
			break;
		case MT_ENDSYNC:
			printf("Sending MT_ENDSYNC to %d.\n", socketfd );
			break;
	}
*/
	while( sent < datasize )
	{
		n = send( socketfd, caux + sent, datasize - sent, 0);
		if( n < 0 ){ break; }
		sent += n;
	}
	return(0);
}
