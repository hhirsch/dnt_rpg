#ifndef _FIFO_H
#define _FIFO_H

#include <stdio.h>
#include <stdlib.h>

typedef struct _fifo_t
{
	int numitens, start, end, maxlen;
	void ** itens;
} fifo_t, * fifo_p_t;

void fifoinit( fifo_p_t fi, int maxlen );
void * fifopush( fifo_p_t fi, void * item );
void * fifopop( fifo_p_t fi );
int fifosize( fifo_p_t fi );

#endif
