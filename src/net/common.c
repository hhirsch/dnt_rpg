#include "common.h"
#include <stdarg.h>

int addrlen = sizeof(struct sockaddr);
const char * _mt_numargs[NUMMT] = MT_STYPES;

int buildmesg( void * data, int type, int obj, ... )
{
	va_list arglist;
	int numfields = strlen( _mt_numargs[type] );
	void * aux = data;
	int * iaux = aux;
	char * caux, * cauxaux;
	double * daux;
	int i;

	iaux[0] = type;
	iaux[1] = obj;
	aux = (void*)(&(iaux[2]));
	
	va_start( arglist, numfields );
	
	for( i = 0; i < numfields; i++ )
	{
		iaux = aux;
		caux = aux;
		daux = aux;
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
	return( aux - data );
}

int senddata( int socketfd, void * data, int datasize )
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
