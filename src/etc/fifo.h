#ifndef _FIFO_H
#define _FIFO_H

#define FIFOSIZE 10

typedef struct _fifo_t
{
	int numitems, start, end;
	void * items [FIFOSIZE];
} fifo_t, * fifo_p_t;

void initfifo( fifo_p_t fifo );
void * push( fifo_p_t fifo, void * item );
void * pop( fifo_p_t fifo );
int fifosize( fifo_p_t fifo );
