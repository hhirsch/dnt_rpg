#include "fifo.h"

void fifoinit( fifo_p_t fi )
{
	fi->numitens = 0;
	fi->start = 0;
	fi->end = 0;
}

void * fifopush( fifo_p_t fi, void * item )
{
	if( ((( fi->end + 1 ) % FIFOSIZE ) == fi->start ) || ( fi->numitens == FIFOSIZE ))
		return(NULL);
	fi->itens[fi->end] = item;
	fi->end = (fi->end + 1) % FIFOSIZE;
	fi->numitens++;
	return( item );
}

void * fifopop( fifo_p_t fi )
{
	void * aux;
	if(( fi->end == fi->start ) || ( fi->numitens == 0 ))
		return(NULL);
	aux = fi->itens[fi->start];
	fi->start = (fi->start+1) % FIFOSIZE;
	fi->numitens--;
	return( aux );
}

int fifosize( fifo_p_t fi )
{
	return( fi->numitens );
}

#ifdef FIFO_TEST
#include <stdio.h>

void printstat( fifo_p_t fi )
{
	printf( "start=%d, end=%d, size=%d, max=%d\n", fi->start, fi->end, fi->numitens, FIFOSIZE );
}

int main( int argc, char ** argv )
{
	int i, j;
	char * bla [4] = { "Vai", "toma", "no", "copo !" };
	fifo_t fi;
	char * bliu;
	
	fifoinit(&fi);
	for( j = 0; j < 10; j++ )
	{
		for( i = 0; i < 4; i++ )
			if ( fifopush( &fi, bla[i] ) == NULL )
				printf( "Couldn't add any more itens. FIFO is full.\n" );

		printstat( &fi );
		for( i = 0; i < 4; i++ )
			if (( bliu = (char *)fifopop( &fi )) == NULL )
				printf( "Couldn't remove any more itens. FIFO is empty.\n" );
			else
				printf( "%s\n", bliu );
		printstat( &fi );
	}
	return(0);
}
#endif
